#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file
setup() {
    # get the containing directory of this file
    # use $BATS_TEST_FILENAME instead of ${BASH_SOURCE[0]} or $0,
    # as those will point to the bats executable's location or the preprocessed file respectively
    DIR="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"
    # make executables in src/ visible to PATH
    PATH="$DIR/../src:$PATH"
}


@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file



@test "Check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}


@test "Exit commands functions properly" {
    run "./dsh" <<EOF                
exit
EOF

    expected_output="dsh2>"
    echo "Output: $output"
    # Check exact match
     [ "$status" -eq 0 ]
}

@test "Cd with no arguments does nothing" {
    current_dir=$(pwd)
    run "./dsh" <<EOF                
cd
EOF

    expected_output="dsh2>"
    echo "DIR: $DIR"
    echo "DIR: $(pwd)"
    # Check exact match
     [ "$status" -eq 0 ]
     [ "$(pwd)" = "$current_dir" ]
}