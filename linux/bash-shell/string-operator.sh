function test
{
    v1="hello"
    v2="frogfootman"
    empty=""
    echo "v1=$v1 v2=$v2 empty=$empty"
    
    echo -e "\n"
    echo "test: \${varname:-word}"
    echo "if varname exists and isn't null, return its value."
    echo "\${v1:-\"don't exit.\"}: "${v1:-"don't exit."}
    echo "otherwise return word. "
    echo "\${empty:-\"don't exit.\"}: "${empty:-"don't exit."}

    echo -e "\n"
    echo "test: \${varname:=word}"
    echo "if varname exists and isn't null, return its value."
    echo "\${v1:=\"don't exit.\"}: "${v1:="don't exit."}
    echo "otherwise set it to word and then return its value."
    echo "before, empty="$empty
    echo "\${empty:=\"return this.\"}: "${empty:="return this."}
    echo "after, empty="$empty
    
    echo -e "\n"
    echo "test: \${varname:+word}"
    echo "if varname exists and isn't null, return word."
    echo "\${v1:+\"exit.\"}: "${v1:+"exit."}
    echo "otherwise return null. "
    echo "\${v100:+\"don't exit.\"}: "${v100:+"don't exit."}

    echo -e "\n"
    echo "test: \${varname:offset:length}"
    echo "It returns the substring of \$varname starting at offset and up to length characters."
    echo "The first character in \$varname is position 0."
    echo "If length is omitted, the substring starts at offset and continues to the end of \$varname."
    echo "test: \${varname:length}, v2="$v2
    echo "\${v2:4}: "${v2:4}
    echo "\${v2:4:4}: "${v2:4:4}
    echo "\${v2:-1:4}: "${v2:-1:4}

    echo -e "\n"
    echo "test: \${varname:?message}"
    echo "if varname exists and isn't null, return its value."
    echo "\${v1:?\"undefined!\"}: "${v1:?"undefined!"}
    echo "otherwise print varname: followed by message, and abort the current command."
    echo "\${v100:?\"undefined!\"}: "${v100:?"undefined!"}

}

test
