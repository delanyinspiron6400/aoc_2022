use std::fs;

fn pos_mod(x: i32) -> i32{
    let ret : i32 = x % 3;
    if ret >= 0 {
        ret
    } else
    {
        ret + 3
    } 
}

fn main() {
    println!("*#*#*# AOC 01.12.2022 *#*#*#");
    let file_content = fs::read_to_string("../input/aoc02.txt").expect("Could not open file.");
    let mut accumulated_score: i32 = 0;
    let mut accumulated_prediction_score: i32 = 0;
    for line in file_content.lines()
    {
        let prediction = line.chars().nth(0).unwrap() as i32 - 'A' as i32;
        let response = line.chars().nth(2).unwrap() as i32 - 'X' as i32;

        accumulated_score += (pos_mod(response + (1 - prediction)) * 3) + (response + 1);
        accumulated_prediction_score += (response * 3) + (pos_mod(prediction + (response - 1)) + 1);
    }

    println!("Task 1: Score {}", accumulated_score);
    println!("Task 2: Score {}", accumulated_prediction_score);
}
