#![ cfg( windows ) ]

use {
    lazy_static::lazy_static,

    deser_hjson,

    serde::Deserialize,

    std::{
        collections::HashMap,

        fs,

        vec::{
            Vec,
        },

        thread,

        sync::{
            Mutex,
        },
    },

    winapi::{
        shared::{
            minwindef::{
                TRUE, BOOL, DWORD, HINSTANCE, HMODULE, LPVOID, FARPROC
            },

            ntdef::{
                NULL, LPCSTR
            },
        },

        um::{
            consoleapi::{
                AllocConsole
            },

            libloaderapi::{
                LoadLibraryA, GetProcAddress
            },
        },
    },
};

#[ derive( Clone, Default, Deserialize, PartialEq, Debug ) ]
struct ModInfo {
    name       : String,
    description: Option< String >,
    update     : Option< String >,
    version    : Option< String >,
    source     : Option< String >,
    main       : String,
    events     : HashMap< String, Option< String > >,
}

macro_rules! v_address_to_function {
    ( $address:expr, $t:ty ) => {
        std::mem::transmute::< *const (), $t >( $address as _ )
    };
}

static mut NETPLAY_INITIALIZATION_ADDRESS: FARPROC = NULL as FARPROC;

lazy_static! {
    #[ derive( Debug ) ]
    static ref ON_EVENT_FUNCTIONS: Mutex< HashMap< String, Vec< usize > > > = Mutex::new( HashMap::new() );
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
    0
}

unsafe fn init() {
    // if cfg!( debug_assertions ) {
        AllocConsole();
    // }

    load_root_directory();
    load_addons_directory();
}

#[ no_mangle ]
pub unsafe extern fn Initialize() {
    if cfg!( debug_assertions ) {
        println!(
            "onInitialization: {:#?}",
            ON_EVENT_FUNCTIONS.lock().unwrap().get( "onInitialization" )
        );
    }

    if let Some( on_initialization_events ) = ON_EVENT_FUNCTIONS.lock().unwrap().get( "onInitialization" ) {
        for &function_address in on_initialization_events.iter() {
            let on_initialization_function = v_address_to_function!(
                function_address,
                extern "C" fn()
            );
    
            on_initialization_function();
        }
    }

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
                // exclude mod loader dll
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
            if cfg!( debug_assertions ) {
                println!(
                    "root directory DLL loaded: {}",
                    path.path().display()
                );
            }

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
            println!( "addon: {:?}", error );

            vec![]
        },
    };

    addons_paths.into_iter().for_each(
        | entry | {
            if cfg!( debug_assertions ) {
                println!(
                    "addon loaded: {}",
                    entry.path().display()
                );
            }

            let mod_info: ModInfo = deser_hjson::from_str(
                &fs::read_to_string(
                    format!(
                        "{}/info.hjson",
                        entry.path().to_str().unwrap()
                    )
                ).unwrap_or_default()
            ).unwrap_or_default();

            if cfg!( debug_assertions ) {
                println!(
                    "name       : {},",
                    mod_info.name
                );
                
                println!(
                    "description: {},",
                    mod_info.description.unwrap_or_default()
                );

                println!(
                    "version    : {},",
                    mod_info.version.unwrap_or_default()
                );
            }

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

            for ( event_name, event ) in mod_info.events.iter() {
                if event != &not_specified {
                    let on_event_old_functions = ON_EVENT_FUNCTIONS.lock().unwrap().insert(
                        String::from( event_name ),
                        vec![
                            GetProcAddress(
                                mod_h_module,
                                format!(
                                    "{}\0",
                                    event.as_ref().unwrap()
                                ).as_ptr() as LPCSTR
                            ) as usize
                        ]
                    );

                    if let Some( on_event_old_functions ) = on_event_old_functions {
                        let mut events = on_event_old_functions.clone();

                        events.push(
                            GetProcAddress(
                                mod_h_module,
                                format!(
                                    "{}\0",
                                    event.as_ref().unwrap()
                                ).as_ptr() as LPCSTR
                            ) as usize
                        );

                        ON_EVENT_FUNCTIONS.lock().unwrap().insert(
                            String::from( event_name ),
                            events
                        );
                    }

                    if cfg!( debug_assertions ) {
                        println!(
                            "{}: {:#?}\n",
                            String::from( event_name ),
                            ON_EVENT_FUNCTIONS.lock().unwrap().get( event_name )
                        );
                    }
                }
            }
        }
    );

    let states_h_module =  LoadLibraryA(
        format!(
            "states.dll\0",
        )
        .as_ptr() as LPCSTR
    ) as HMODULE;

    let mut on_event_functions: HashMap< String, extern "C" fn( usize, *const usize ) > = HashMap::new();

    on_event_functions.insert(
        String::from( "onGameStarted" ),
        v_address_to_function!(
            GetProcAddress(
                states_h_module,
                "onGameStarted\0".as_ptr() as LPCSTR
            ) as FARPROC,
            extern "C" fn( usize, *const usize )
        )
    );

    on_event_functions.insert(
        String::from( "onHosted" ),
        v_address_to_function!(
            GetProcAddress(
                states_h_module,
                "onHosted\0".as_ptr() as LPCSTR
            ) as FARPROC,
            extern "C" fn( usize, *const usize )
        )
    );

    on_event_functions.insert(
        String::from( "onConnection" ),
        v_address_to_function!(
            GetProcAddress(
                states_h_module,
                "onConnection\0".as_ptr() as LPCSTR
            ) as FARPROC,
            extern "C" fn( usize, *const usize )
        )
    );

    thread::spawn(
        move || {
            for ( event_name, function_addresses ) in ON_EVENT_FUNCTIONS.lock().unwrap().iter() {
                if event_name != "onInitialization" {
                    if cfg!( debug_assertions ) {
                        println!(
                            "{} : {} -> {:#02X?}",
                            event_name,
                            function_addresses
                                .len(),
                            function_addresses
                                .to_owned()
                        );
                    }

                    if let Some( on_event_function ) = on_event_functions.get( event_name ) {
                        on_event_function(
                            function_addresses
                                .len(),
                            function_addresses
                                .to_owned().as_ptr()
                        );
                    }
                }
            }
        }
    );
}
