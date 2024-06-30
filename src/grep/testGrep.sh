#!/bin/bash
grep="grep "
s21_grep="./s21_grep"
SUCCESS=0
FAIL=0
COUNTER=0

# RED="\033[31m"
# GREEN="\033[32m"
# BLUE="\033[36m"
# ENDCOLOR="\033[0m"

grep_c="grep.c"

testing(){
    ./s21_grep $var $grep_c> test_s21_grep.log 
    grep $var $grep_c > test_grep.log
    if cmp -s test_s21_grep.log test_grep.log
    then
    (( SUCCESS++ ))
    (( COUNTER++ ))

    echo -e "$BLUE$s21_grep $var $grep_c ${ENDCOLOR}\n${GREEN}Test №$COUNTER SUCCESS${ENDCOLOR}"
    else
    (( FAIL++ ))
    (( COUNTER++ ))

    echo -e "$BLUE$s21_grep $var $grep_c ${ENDCOLOR}\n${RED}Test №$COUNTER FAIL${ENDCOLOR}"
    fi
    # rm -f ./test_s21_grep.log ./test_grep.log
}

# проверки с одними флагами без значений
for var1 in v c l i h o n # 7 из 10
do  
    var="0 -$var1"
    testing "$var"
done

# проверки с одними флагами без значений двух файлов
for var1 in v c l i h # 7 из 10
do  
    var="0 -$var1 grep.c"
    testing "$var grep.c"
done

# проверки для флага s который глушит сообщения об ошибках с файлом
for var1 in grep.c grep.h gre.b grep.tlsdf s21  # 8 из 10
do  
    var="A -s $var1"
    testing "$var"
done

# проверка на е флаг который задает паттерны 9 из 10
for var1 in " '[0-9]'" " '[a-z]'" "'a'" " '[0-9].*'"
do  
    var="-e$var1"
    testing "$var"
done

# проверка на е флаг который задает паттерны
for var1 in " '[0-9]'" " '[a-z]'" "'a'" " '[0-9].*'"
do
    for var2 in " 1" " '[a-x]'" "'vo'" "'.*ap.*'"    
    do  
        var="-e$var1 -e$var2"
        testing "$var"
    done
done

# проверка на е флаг который задает паттерны 9 из 10
for var1 in "0.txt" "1.txt" "2.txt" "3.txt"
do  
    var="-f $var1"
    testing "$var"
done

var="-f 0.txt -f 2.txt"
        testing "$var"

var="-f 1.txt -f 3.txt"
        testing "$var"

echo -e "${GREEN}SUCCESS $SUCCESS ${ENDCOLOR} | ${RED} FAILED $FAIL${ENDCOLOR}";