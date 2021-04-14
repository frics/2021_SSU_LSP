#!/bin/bash
declare -i monthDay
declare -i month
declare -i day
monthDay=(31 28 31 30 31 30 31 31 30 31 30 31)
DAY=(금 토 일 월 화 수 목)

echo "INPUT MONTH : "
read month
echo "INPUT DAY : "
read day
if(( ${monthDay[$month-1]} < $day ))
then
    echo "잘못된 날짜입니다."
else
    echo "$month월 $day일 "
    #요일 계산 부분
    for((i=0;i<$month-1;i++))
    do
        day=$day+${monthDay[$i]}
    done
    let index=$day%7-1
    echo "${DAY[$index]}요일"
fi