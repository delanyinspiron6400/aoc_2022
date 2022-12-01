use std::fs;

fn main() {
    println!("*#*#*# AOC 01.12.2022 *#*#*#");
    let file_content = fs::read_to_string("../input/aoc01.txt").expect("Could not open file.");
    let mut calories_per_elf : Vec<i32> = Vec::new();
    let mut calory_per_elf: i32 = 0;
    for line in file_content.lines()
    {
        let calory: i32 = match line.trim().parse::<i32>(){
            Ok(value) => value,
            Err(_error) => -1,
        };
        
        if calory != -1{
            calory_per_elf += calory;
        }
        else {
            calories_per_elf.push(calory_per_elf);
            calory_per_elf = 0;
        }
    }

    calories_per_elf.sort();
    calories_per_elf.reverse();

    println!("Task 1: {} kcal", calories_per_elf[0]);
    println!("Task 2: {} kcal", calories_per_elf[0] + calories_per_elf[1] + calories_per_elf[2]);

}
