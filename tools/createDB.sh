#!/bin/bash
USER_NAME="root"
USER_PWD="123"
DB_NAME="michelledata"
DB_HOST="localhost"
SQL_DIR="local_sql"

function echo_red
{
    echo -e "\033[1;31;40mNOTE by $SCRIPT_NAME : $1 \033[0m"
}

function echo_green
{
    echo -e "\033[1;32;40mNOTE by $SCRIPT_NAME : $1 \033[0m"
}

function import_file
{
    if [ -f $1 ]; then
        echo_green "Will import sql file : $1"
        if [ $USER_PWD ]; then
            echo "mysql -u$USER_NAME -h$DB_HOST $DB_NAME -p$USER_PWD < $1"
            mysql -u$USER_NAME -h$DB_HOST $DB_NAME -p$USER_PWD < $1
        else
            echo "mysql -u$USER_NAME -h$DB_HOST $DB_NAME  < $1"
            mysql -u$USER_NAME -h$DB_HOST $DB_NAME  < $1
        fi
    else
        echo_red "$1 is not found. Pls run create_files.sh at first!"
        exit 1
    fi
}


#for file in ${SQL_FILES[*]}
for file in `ls ${SQL_DIR}/*.sql`
do
    import_file $file
done

echo_green "Has import all sql files"

