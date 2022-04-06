const DESTINATION_MASK: u16 = 0xFF00;
const ORIGIN_MASK: u16 = 0x00FF;

/// Parses a move byte to a string notation (origin-destination)
/// using UCI-like notation
pub fn parse(move_byte: u16) -> Result<String, String> {
    let (origin, destination) = (move_byte & ORIGIN_MASK, (move_byte & DESTINATION_MASK) >> 8);

    if !(0..64).contains(&origin) || !(0..=64).contains(&destination) { Err(String::from("Invalid move")) }
    else { Ok(square_string_from_index(origin as u8) + &square_string_from_index(destination as u8)) }
}

/// Returns the string representation of a square given
/// its index
pub fn square_string_from_index(index: u8) -> String {
    if !(0..64).contains(&index) { panic!("Tried parsing an out of bounds index") }
    String::from((('a' as u8) + index%8) as char) + &(index/8 + 1).to_string()
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn parser_test() {
        assert_eq!(parse(0x040C), Ok(String::from("e1e2")));
        assert_eq!(parse(0x003F), Ok(String::from("a1h8")));
    }

    #[test]
    fn parser_test_invalid() {
        assert_eq!(parse(0x0054), Err(String::from("Invalid move")));
        assert_eq!(parse(0x5400), Err(String::from("Invalid move")));
        assert_eq!(parse(0x5454), Err(String::from("Invalid move")))
    }

    #[test]
    fn square_string_from_index_tests() {
        assert_eq!(square_string_from_index(0), "a1");
        assert_eq!(square_string_from_index(28), "e4");
        assert_eq!(square_string_from_index(63), "h8");
    }

    #[test]
    #[should_panic]
    fn square_string_from_index_panics() {
        square_string_from_index(64);
    }
}