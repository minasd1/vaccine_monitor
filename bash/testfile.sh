#! /bin/bash  
args=("$@")

if [ "$#" -ne 4 ]               #number of arguments must be equal to 4
then
    echo "You must give 4 arguments!"
    exit 1
fi

numLines=${args[2]}             #number of inputFile lines

if [ "$numLines" -le 0 ]
then
    echo "3rd argument must greater than 0!"
    exit 1
fi

duplicatesAllowed=${args[3]}

if ! [[ "$duplicatesAllowed" =~ ^[0-9]+$ ]]
then
    echo "4th argument must be an integer!"
    exit 1
fi


touch inputFiletmp; rm inputFiletmp; touch inputFiletmp;
touch inputFile; rm inputFile; touch inputFile;

viruseslist=()

fileName=${args[0]}
#if given fileName exists
if [[ -f "$fileName" ]]
then                            #read file line by line
    exec < "$fileName"      

    while read line
    do
        viruseslist+=($line)    ##attend to viruseslist        
    done
else
    echo "$fileName doesn't exist"
    exit 1
fi


countrieslist=()

fileName2=${args[1]}
#if given fileName exists
if [[ -f "$fileName2" ]]
then                            #read file line by line
    exec < "$fileName2" 

    while read line
    do
        countrieslist+=($line)  #attend to countrieslist
    done
else
    echo "$fileName2 doesn't exist"
    exit 1
fi

idlist=($(seq 1 1 10000))

num_viruses=${#viruseslist[@]}
num_countries=${#countrieslist[@]}


MINNUM=1                         #min id
MAXNUM=9999                      #max id
MINYEAR=1990
MAXYEAR=2040
MINSTRSIZE=3                     #size of firstname and lastname strings
MAXSTRSIZE=12
MAXAGE=120
isvaccinated=('YES' 'NO')

duplicatelist=()                #list that holds duplicate records

if [ "$duplicatesAllowed" -eq 0 ]
then
    iterations=$numLines
else
    iterations=$((numLines/2))
fi


count=0

for (( i=0; i<iterations; i++ ))
do  
    
    if [[ $count -le MAXNUM ]]
    then
        idposition=$(($count+RANDOM%((MAXNUM+1-$count))))       #pick random position in idlist
        id=${idlist[idposition]}                                #get its' value
        idlist[idposition]=${idlist[count]}                     #swap values with the first index with not already used value 
        idlist[count]=$id
        count=$((count+1))                                      #increment count so we don't have to choose the same value again
    else
        echo "All possible ids inserted. Insertion terminated!"
    fi    

    country=${countrieslist[$((RANDOM%num_countries))]}     #pick random country
    age=$((RANDOM%MAXAGE+1))                                #pick random age (range [1,120])
    virus=${viruseslist[$((RANDOM%num_viruses))]}           #pick random virus
    isvac=${isvaccinated[$((RANDOM%2))]}                    #pick YES or NO randomly
    probability=RANDOM%100                                  #probability of a notvaccinated record having a date of vaccination
    
    
    
    echo -n "$id " >> inputFiletmp                                 #insert id value    

    fnamelen=$((MINSTRSIZE+RANDOM%((MAXSTRSIZE+1-MINSTRSIZE))))
    tr -dc A-Za-z </dev/urandom | head -c $fnamelen >> inputFiletmp  #insert firstname
    echo -n " " >> inputFiletmp
    lnamelen=$((MINSTRSIZE+RANDOM%((MAXSTRSIZE+1-MINSTRSIZE))))
    tr -dc A-Za-z </dev/urandom | head -c $lnamelen >> inputFiletmp  #insert lastname
    echo -n " " >> inputFiletmp

    echo -n "$country $age" >> inputFiletmp                        #insert country, age
    
    echo -n " " >> inputFiletmp
    echo -n "$virus $isvac" >> inputFiletmp                        #insert virus, yes/no

    if [[ $isvac == ${isvaccinated[0]} || $probability -le 5 ]] #if person is vaccinated or probability is true, process date argument
    then
        year=$((MINYEAR+RANDOM%((MAXYEAR+1-MINYEAR))))
        month=$((RANDOM%12+1))
        #If month is February, check if year is leap, had to do this to prevent errors of invalid dates from occuring during execution       
        if [[ $month -eq 2 ]]
        then
            if [[ $year%4 -ne 0 ]]
            then
                day=$((RANDOM%28+1))
            elif [[ $year%100 -ne 0 ]]
            then
                day=$((RANDOM%29+1))
            elif [[ $year%100 -ne 0 ]]
            then
                day=$((RANDOM%28+1))
            else
                day=$((RANDOM%29+1))
            fi
        else
            day=$((RANDOM%30+1))
        fi

        echo -n " " >> inputFiletmp
        date -d "$year-$month-$day" '+%d-%m-%Y' >> inputFiletmp
    else
        echo "" >> inputFiletmp
    fi
    
done

sort -n -k 1 inputFiletmp > inputFile     #sort inputFile to be more clear
rm inputFiletmp;                         

if [ "$duplicatesAllowed" -eq 0 ]           #no duplicates allowed
then                                        #terminate
    exit 0
else                                        #if duplicates allowed
    
    awk '{print $1, $2, $3, $4, $5}' inputFile > recordfile     #get all the different citizen data from file
    mapfile -t duplicatelist < recordfile                       #make a list of them
    rm recordfile;
    
fi


num_citizens=${#duplicatelist[@]}                       #number of different citizen data
num=$(wc -l < inputFile)                                
iterations=$((numLines - num))                          #fill the rest lines of file with records with citizens already been given


for (( i=0; i<iterations; i++ ))
do
    
    citizen=${duplicatelist[$((RANDOM%num_citizens))]}  #pick a random citizen from list
    virus=${viruseslist[$((RANDOM%num_viruses))]}       #pick a random virus
    isvac=${isvaccinated[$((RANDOM%2))]}                #pick YES or NO
    probability=RANDOM%100                              #probability of a notvaccinated record having a date of vaccination
    

    echo -n "$citizen $virus $isvac" >> inputFile       #insert to file

    if [[ $isvac == ${isvaccinated[0]} || $probability -le 5 ]] #process date argument
    then
        year=$((MINYEAR+RANDOM%((MAXYEAR+1-MINYEAR))))
        month=$((RANDOM%12+1))
        #If month is February, check if year is leap, had to do this to prevent errors of invalid dates from occuring during execution       
        if [[ $month -eq 2 ]]
        then
            if [[ $year%4 -ne 0 ]]
            then
                day=$((RANDOM%28+1))
            elif [[ $year%100 -ne 0 ]]
            then
                day=$((RANDOM%29+1))
            elif [[ $year%100 -ne 0 ]]
            then
                day=$((RANDOM%28+1))
            else
                day=$((RANDOM%29+1))
            fi
        else
            day=$((RANDOM%30+1))
        fi

        echo -n " " >> inputFile
        date -d "$year-$month-$day" '+%d-%m-%Y' >> inputFile
    else
        echo "" >> inputFile
    fi

done
  
sort -n -k 1 -o inputFile inputFile     #sort inputFile to be more clear

exit 0

