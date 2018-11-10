#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 4

#[EN]Array with names of functionality /[RU]Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Сменить текущий каталог" 
						"Выдать список пользователей, имеющих хотя бы один процесс" 
						"Создать файл" 
						"Скопировать файл" 
						"Выйти из программы")

#[EN]Initialize variable with path to log file /[RU]Инициализируем переменную с расположением лог файлов
SCRIPTLOGS="$PWD/logs.txt"
echo $SCRIPTLOGS

#[EN]Create log file if he doesn't exist, else print information about existing of the file 
#[RU]Создание лог файла в случее его отсутствия в каталоге со скриптом, иначе выводит сообщение о существовании такового файла
createLogFile() {
	if test -f "$SCRIPTLOGS"; then
		echo "Log file is exist"
	else
		touch "$SCRIPTLOGS"
	fi 
}

#[EN]Print to console "Main Menu" /[RU]Выводит в консоль "Главное меню"
printMenu() {
	for i in ${!systemCall[*]}
	do
		indexForUser=$[$i+1]
		echo $indexForUser"." ${systemCall[$i]}
	done
}

#[EN]Print to console "Current directory" /[RU]Выводит в консоль "Текущий каталог"
printCurrentDir() {
	echo "Текущий каталог: "
	pwd
}

#[EN]Go to Directory /[RU]Запрашивает путь к каталогу, затем переходит в указанный каталог
changeDir() {
	echo "Введите путь к каталогу"
	read -r pathToDir
	eval "cd $pathToDir 2>>\"$SCRIPTLOGS\" || echo Переход в каталог \"$pathToDir\" не возможен>&2"
}

#[EN]Print to console Users with equals or greater than 1 /[RU]Выводит в консоль пользователей имеющих хотя бы один процесс
showUsersProcess() {
	echo "Пользователи имеющие процессы: "
	ps -eo user | grep -v "USER" | sort | uniq
}

#[EN]Take name of file, then create file /[RU]Запрашивает имя файла, после чего создает файл с указанным именем
createFile() {
	echo "Введите имя файла"
	read -r nameFile
	eval "touch $nameFile 2>>\"$SCRIPTLOGS\" || echo Неудалось создать файл \"$nameFile\">&2"
}

#[EN]Copy the file /[RU] Копирует указанный файл
copyFile() {
	echo "Введите имя копируемого файла"
	read -r nameFile
	echo "Введите целевой каталог"
	read -r destinationPath
	eval "cp $nameFile $destinationPath 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#[EN]The begin of the functional part of the script /[RU]Начало исполнения функциональной части скрипта
createLogFile

printMenu

while :
do
	
	read -n 1 key
	echo

	case $key in
		1)printCurrentDir;;
		2)changeDir;;
		3)showUsersProcess;;
		4)createFile;;
		5)copyFile;;
		6)
			echo "Goodbye"
			break;;
		*)
			#[EN]Handling an event with an unspecified index in a case and checking for a keystroke by pressing Ctrl-D
			#[RU]Обработка события с неуказанным в case индексом с проверкой на ввод сочетания клавишь Ctrl-D
    		line=$line$key
   			printf -v key_code "%d" "'$key"
    		if [ $key_code -eq 4 ]; then
        		echo "Ctrl-D pressed!"
        		break
    		fi
    		if [ $key_code -eq 0 ]; then
    			printMenu
    		else     			
    			echo "Не верный индекс"
    		fi;;
	esac
done
