#![ cfg( windows ) ]

use {
    deser_hjson,

    serde::Deserialize,

    std::{
        collections::HashMap,

        fs,

        vec::{
            Vec,
        },

        thread,
    },

    winapi::{
        shared::{
            minwindef::{ // UINT
                TRUE, BOOL, DWORD, HINSTANCE, HMODULE, LPVOID, FARPROC
            },

            // windef::{
            //     HWND
            // },

            ntdef::{
                NULL, LPCSTR
            },
        },

        um::{
            // winuser::{
            //     MessageBoxA
            // },

            consoleapi::{
                AllocConsole
            },

            libloaderapi::{
                LoadLibraryA, GetProcAddress
            },
        },
    },
};

#[ derive( Default, Deserialize, PartialEq, Debug ) ]
struct ModInfo {
    name       : String,
    description: Option< String >,
    update     : Option< String >,
    version    : Option< String >,
    source     : Option< String >,
    main       : String,
    events     : HashMap< String, Option< String > >,
}

static mut ON_INITIALIZATION: Vec< FARPROC > = Vec::new();

static mut NETPLAY_INITIALIZATION_ADDRESS: FARPROC = NULL as FARPROC;

macro_rules! v_address_to_function {
    ( $address:expr, $t:ty ) => {
        std::mem::transmute::< *const (), $t >( $address as _ )
    };
}

/// Rust doesn't officially support a "life before main()",
/// though it is unclear what that that means exactly for DllMain.
#[ no_mangle ]
#[ allow( non_snake_case, unused_variables ) ]
unsafe extern "system" fn DllMain(
    dll_module : HINSTANCE,
    call_reason: DWORD,
    reserved   : LPVOID
) -> BOOL {
    const DLL_PROCESS_ATTACH: DWORD = 1;
    const DLL_PROCESS_DETACH: DWORD = 0;

    match call_reason {
        DLL_PROCESS_ATTACH => init(),
        DLL_PROCESS_DETACH => (),
        _                  => ()
    }

    TRUE
}

#[ no_mangle ]
pub unsafe extern fn thcrap_plugin_init() -> u32 {
    // AllocConsole();
    println!( "Hello from thcrap!" );

    // init();

    0
}

unsafe fn init() {
    AllocConsole();
    println!( "Hello from demo_init()!" );

    load_root_directory();
    load_addons_directory();
}

#[ no_mangle ]
pub unsafe extern fn Initialize() {
    // const WINDOW_TEXT: LPCSTR = "world!\0".as_ptr() as LPCSTR;
    // const WINDOW_NAME: LPCSTR = "Hello!\0".as_ptr() as LPCSTR;

    // MessageBoxA(
    //     NULL as HWND,
    //     WINDOW_TEXT,
    //     WINDOW_NAME,
    //     NULL as UINT
    // );

    // let first_player_health: *const u32 = 0x3F77BED8 as *const u32;

    // println!( *first_player_health );

    ( &ON_INITIALIZATION ).into_iter().for_each(
        | function_address | {
            let on_initialization_function = v_address_to_function!(
                *function_address,
                extern "C" fn()
            );
    
            on_initialization_function();
        }
    );

    if NETPLAY_INITIALIZATION_ADDRESS != NULL as FARPROC {
        let netplay_initialization = v_address_to_function!(
            NETPLAY_INITIALIZATION_ADDRESS,
            extern "C" fn()
        );

        netplay_initialization();
    }
}

unsafe fn load_root_directory() {
    let dll_paths: Vec< fs::DirEntry > = fs::read_dir( "./" )
        .unwrap()
        .into_iter()
        .filter(
            | result | {
                result.is_ok()
            }
        )
        .map(
            | result | {
                result.unwrap()
            }
        )
        .filter(
            | path | {
                if path.path().to_str().unwrap() == "./Netplay.dll" {
                    return false;
                }

                match path.path().extension() {
                    Some( extension ) => extension == "dll",
                    _                 => false,
                }
            }
        )
        .collect();

    dll_paths.into_iter().for_each(
        | path | {
            println!(
                "root directory DLL loaded: {}",
                path.path().display()
            );

            if NETPLAY_INITIALIZATION_ADDRESS == NULL as FARPROC {
                NETPLAY_INITIALIZATION_ADDRESS = GetProcAddress(
                    LoadLibraryA(
                        format!(
                            "{}\0",
                            path
                                .path()
                                .to_str()
                                .unwrap()
                        )
                        .as_ptr() as LPCSTR
                    ) as HMODULE,
                    "Initialize\0".as_ptr() as LPCSTR
                ) as FARPROC;
            }
        }
    );
}

unsafe fn load_addons_directory() {
    let mut on_game_started_events: Vec< usize > = Vec::new();
    let mut on_hosted_events      : Vec< usize > = Vec::new();
    let mut on_connection_events  : Vec< usize > = Vec::new();

    let addons_paths: Vec< fs::DirEntry > = match fs::read_dir( "addons" ) {
        Ok( directory ) => {
            directory
            .into_iter()
            .filter(
                | result | {
                    result.is_ok()
                }
            )
            .map(
                | result | {
                    result.unwrap()
                }
            )
            .filter(
                | entry | {
                    entry.path().is_dir()
                }
            )
            .collect()
        },
        Err( error ) => {
            println!( "addons: {:?}", error );

            vec![]
        },
    };

    addons_paths.into_iter().for_each(
        | entry | {
            println!(
                "addon loaded: {}",
                entry.path().display()
            );

            let mod_info: ModInfo = deser_hjson::from_str(
                &fs::read_to_string(
                    format!(
                        "{}/info.hjson",
                        entry.path().to_str().unwrap()
                    )
                ).unwrap_or_default()
            ).unwrap_or_default();

            println!(
                "name       : {},",
                &mod_info.name
            );
            
            println!(
                "description: {},",
                &mod_info.description.unwrap_or_default()
            );

            println!(
                "update     : {},",
                &mod_info.update.unwrap_or_default()
            );

            println!(
                "version    : {},",
                &mod_info.version.unwrap_or_default()
            );

            println!(
                "source     : {},",
                &mod_info.source.unwrap_or_default()
            );

            println!(
                "main       : {},",
                &mod_info.main
            );

            println!(
                "events     : {:?}",
                &mod_info.events
            );

            let mod_h_module: HMODULE = LoadLibraryA(
                format!(
                    "{}\\{}\0",
                    entry.path().to_str().unwrap(),
                    &mod_info.main
                )
                .as_ptr() as LPCSTR
            ) as HMODULE;

            let not_specified = Option::from(
                String::from( "Not specified" )
            );

            let event = mod_info.events.get( "onInitializatiom" )
            .unwrap_or(
                &not_specified
            );

            if event != &not_specified {
                ON_INITIALIZATION.push(
                    GetProcAddress(
                        mod_h_module,
                        format!( "{}\0", event.as_ref().unwrap() ).as_ptr() as LPCSTR
                    ) as FARPROC
                );
            }

            let event = mod_info.events.get( "onGameStarted" )
            .unwrap_or(
                &not_specified
            );

            if event != &not_specified {
                on_game_started_events.push(
                    GetProcAddress(
                        mod_h_module,
                        format!( "{}\0", event.as_ref().unwrap() ).as_ptr() as LPCSTR
                    ) as usize
                );
            }

            let event = mod_info.events.get( "onHosted" )
            .unwrap_or(
                &not_specified
            );

            if event != &not_specified {
                on_hosted_events.push(
                    GetProcAddress(
                        mod_h_module,
                        format!( "{}\0", event.as_ref().unwrap() ).as_ptr() as LPCSTR
                    ) as usize
                );
            }

            let event = mod_info.events.get( "onConnection" )
            .unwrap_or(
                &not_specified
            );

            if event != &not_specified {
                on_connection_events.push(
                    GetProcAddress(
                        mod_h_module,
                        format!( "{}\0", event.as_ref().unwrap() ).as_ptr() as LPCSTR
                    ) as usize
                );
            }
        }
    );

    let states_h_module =  LoadLibraryA(
        format!(
            "states.dll\0",
        )
        .as_ptr() as LPCSTR
    ) as HMODULE;

    let on_game_started_function = v_address_to_function!(
        GetProcAddress(
            states_h_module,
            "onGameStarted\0".as_ptr() as LPCSTR
        ) as FARPROC,
        extern "C" fn( usize, Vec::< usize > )
    );

    let on_hosted_function = v_address_to_function!(
        GetProcAddress(
            states_h_module,
            "onHosted\0".as_ptr() as LPCSTR
        ) as FARPROC,
        extern "C" fn( usize, Vec::< usize > )
    );

    let on_connection_function = v_address_to_function!(
        GetProcAddress(
            states_h_module,
            "onConnection\0".as_ptr() as LPCSTR
        ) as FARPROC,
        extern "C" fn( usize, Vec::< usize > )
    );

    println!(
        "started  : {} -> {:#02X?}",
        on_game_started_events.len(), on_game_started_events
    );

    println!(
        "hosted    : {} -> {:#02X?}",
        on_hosted_events.len(), on_hosted_events
    );

    println!(
        "connection: {} -> {:#02X?}",
        on_connection_events.len(), on_connection_events
    );

    thread::spawn(
        move || {
            on_game_started_function( on_game_started_events.len(), on_game_started_events );
            on_hosted_function( on_hosted_events.len(), on_hosted_events );
            on_connection_function( on_connection_events.len(), on_connection_events );
        }
    );
}
