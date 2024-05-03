mkdir -p /home/oleg/OS_Labs/Lab1/Log
touch /home/oleg/OS_Labs/Lab1/Log/log_file.txt
{
    echo "Date: $(date)"
    echo "User: $USER"
    echo "PC: $HOSTNAME"
    echo "Version: $(uname -r)"
} >> /home/oleg/OS_Labs/Lab1/Log/log_file.txt

cd /home/oleg/OS_Labs/Lab1
make clean
tar -czf Lab1.tar.gz .
mv Lab1.tar.gz /home/oleg/OS_Labs/Lab1/Log
{
    echo "Size: $(du -sh /home/oleg/OS_Labs/Lab1/Log/Lab1.tar.gz | cut -f1)"
    echo "Big: $(du -ah | sort -rh | head -n 1)"
    echo "Small: $(du -ah | sort -h | head -n 1)"
} >> /home/oleg/OS_Labs/Lab1/Log/log_file.txt

chmod -R g+w /home/oleg
chmod -R o+rx /home/oleg

echo "Check content:"
select obj in /home/oleg/OS_Labs/Lab1/*; do
    if [ "$REPLY" -eq 0 ]; then
        echo "Exit"
        break
    elif [ -f "$obj" ]; then
        cat "$obj"
    elif [ -d "$obj" ]; then
        ls -l "$obj"
    else
        echo "Wrong ID"
    fi
done