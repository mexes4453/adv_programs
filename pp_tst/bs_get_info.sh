#!/bin/bash
V_FLAG=$1
V_ARG=$2 

function GetIdForTakaNodeOnTesterModel(){
    grep $1  NodeVersionsHW_20260130_id.ps5000.log | awk '{print $1}'
}

function example(){
func_call=$( GetIdForTakaNodeOnTesterModel "$filePath" "$VAR_INTERVAL"); status=$?;
        if [[ $status -eq 0 ]]; then
             func__Logger "===> transmitting $dbFile -> $fileName"; 
             func__TransmitFile "$VAR_SERVER" "$PATH_DST/$fileName" "$filePath";
        else
             func__Logger "===> no update"; 
        fi
}

ARGS=$1
FILE_INPUT=inp.txt
FILE_DB=exp.txt
FILE_TMP=tmp.txt
APP_EXEC=python3
APP_FILE=stat.py


ARGS_GREP=''
ARGS_LIST=( "$@" )
EXEC_GREP=""
EXEC_STATUS=true

# make a copy of the record file
# The new file will be used as the work


#function for help message 
function ShowMsg() {
        echo usage;
        echo -----
        echo "-h : show help message."
        echo "-t : get taka id mapping in tester model file."
        echo "   + example: $ ./bs_get_info.sh -t "00:00:00:1f" "
        
        echo "-s : get tester slot given the id found in tester model file."
        echo "-h : get the hla serial number with slot nbr as input."
        echo "   + example: $ ./bs_get_info.sh -s 4"
        echo
        echo "-c : get the chmod serial number with id in tester model file as input."
        echo "   + example: $ ./bs_get_info.sh -c 527"
        echo
        echo
}



function FormatRecordFile() {
    # Remove excessive whitespace surrounding texts within delimited by pipe symbol
    cat $FILE_DB | sed -E 's/[[:space:]]*\|[[:space:]]*/ | /g' > $FILE_TMP

    # format each line properly with column command
    cat $FILE_TMP | column -t -s "|" -o "|" > $FILE_INPUT

    # rebuild record file
    head -n 1 $FILE_INPUT > $FILE_TMP              # copy header first to new file
    sed -n '2~1p' $FILE_INPUT | sort  >> $FILE_TMP # copy the rest of file content to new file after sorting
    cp $FILE_TMP $FILE_DB                          # store as main record
}


# Iterate over script arguments
for (( i=0; i<$#; i++ )); 
do
    case "${ARGS_LIST[$i]}" in
        "-h")  # show help message
        ShowMsg
        EXEC_STATUS=false
    ;;
        "-t")  # format db file
        i=$(( i + 1 ))
        exec_arg="${ARGS_LIST[$i]}"
        GetIdForTakaNodeOnTesterModel $exec_arg
        EXEC_STATUS=false
    ;;
        "-b")  # format db file
        CreateRecordFileBakup
        EXEC_STATUS=false
    ;;
        *)
        echo "wrong input: no flag or option found. For help use '-h'"
        EXEC_STATUS=false
        echo
    esac
done

