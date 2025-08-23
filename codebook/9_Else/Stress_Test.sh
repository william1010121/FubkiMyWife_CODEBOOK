i=0
while true 
do
    echo $i
    i=$(($i+1))
    python3 gen.py > input.txt
    python3 brute.py < input.txt > brute.txt
    ./sol < input.txt > output.txt
    diff output.txt brute.txt
    if [ $? -ne 0 ]
    then
        break
    fi
done