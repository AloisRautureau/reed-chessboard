use serialport::SerialPort;

pub mod serial_communication;
pub mod move_parser;

const VERSION: &str = env!("CARGO_PKG_VERSION");

/// Initializes the driver and tries to find a compatible device
pub fn init() -> Box<dyn SerialPort> {
    println!("rcd v{} by Aloïs 'baub' RAUTUREAU, 2022", VERSION);
    loop {
        if let Ok(p) = serial_communication::connect() {
            break p;
        }
    }
}