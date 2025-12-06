if [ $# -eq 0 ]; then
    >&2 echo "Not provided project directory as a argument"
    exit 1
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_NAME=$( echo $1 | rev | cut -d'/' -f 2 | rev )
cd ${SCRIPT_DIR}/${PROJECT_NAME}
echo ${PROJECT_NAME}
g++ -std=c++23 -pthread -DPARALLEL -o ${PROJECT_NAME}.o main.cpp && ./${PROJECT_NAME}.o