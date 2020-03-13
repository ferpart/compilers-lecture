#!/usr/bin/python3
"""Lab 01 python code"""

import re
import sys

def main():
    """main function for class asm-analytics.sh"""
    file_scrubber(sys.argv[1])

def file_scrubber(file_name: str):
    """Scrubber for finding information on the provided file"""
    file = open(file_name, "r")
    if file.mode == "r":
        instruction_scrubber_temp = []
        instruction_scrubber = []

        function_scrubber = []

        line = file.readline()
        while line:
            if (line[0] == " " and line[1] == " "):
                instruction_scrubber_temp.append(re.split(r"  +", line.replace("\t", "  ")
                                                          .replace("\n", "")))
            elif line[0].isdigit():
                function_scrubber.append(re.split(r"\s", line.replace("\n", "")
                                                  .replace(":", "")
                                                  .replace("<", "")
                                                  .replace(">", "")))
            line = file.readline()

        instruction_scrubber_temp_size = len(instruction_scrubber_temp)

        for i in range(instruction_scrubber_temp_size):
            if len(instruction_scrubber_temp[i]) > 1:
                if instruction_scrubber_temp[i][len(instruction_scrubber_temp[i])-1] == "":
                    instruction_scrubber.append(instruction_scrubber_temp[i]
                                                [1:len(instruction_scrubber_temp[i])-1])
                else:
                    instruction_scrubber.append(instruction_scrubber_temp[i][1:])
        message(instruction_scrubber, function_scrubber)
    file.close()
    return

def message(instruction_lines: list, function_lines: list):
    """method to display messages on terminal"""

    print("Hi, this is the output of the analysis:")
    print("\tThere are a total of %d instructions." %(len(instruction_lines)))
    instruction_counter(instruction_lines)
    print("\t\tYou have %d functions:" %(len(function_lines)))
    function_counter(function_lines)

def instruction_counter(instruction_lines: list):
    """method to count how many times an instruction is executed"""
    ammounts = {}
    for i in instruction_lines:
        if len(i) > 2:
            appended_elem = i[2].split(" ")
            if ammounts:
                if appended_elem[0] not in ammounts:
                    ammounts[appended_elem[0]] = 1
                else:
                    ammounts[appended_elem[0]] += 1
            else:
                ammounts[appended_elem[0]] = 1
    ammounts_sorted = sorted(ammounts.items(), key=lambda x: x[1], reverse=True)
    ammounts_sorted_len = len(ammounts_sorted)
    print("\t\tYou have %d kind of instructions in this object file:"
          %(ammounts_sorted_len))
    for i in ammounts_sorted:
        print("\t\t\t%s \t\t\t\t : Executed %d times" %(i[0], i[1]))

def function_counter(function_lines: list):
    """method to count how many times functions appear"""

    for i in function_lines:
        print("\t\t\t%s \t\t\t\t : Located at %s addr" %(i[1], i[0]))

if __name__ == "__main__":
    main()
