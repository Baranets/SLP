#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 5

#[EN]Array with names of functionality /[RU]Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Напечатать содержимое текущего каталога" 
						"Вывести текущую дату и время в формате Wed Feb 2 01:01 MSK 2015" 
						"Вывести содержимое файла на экран" 
						"Удалить файл" 
						"Выйти из программы")

#[EN]Initialize variable with path to log file /[RU]Инициализируем переменную с расположением лог файлов
SCRIPTLOGS="$PWD/logs.txt"

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

#[EN]Print to console "Current directory content" /[RU]Выводит в консоль "Содержание текущего каталога"
printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

#[EN]Print to console "Current formatted date" /[RU]Выводит в консоль "Текущую форматированную дату"
currentDatePrint() {
	echo "Current date is "
	date '+%a %b %e %H:%M %Z %Y'
}

#[EN]Print to console content in file /[RU]Выводит в консоль содержимое файла
printFile() {
	echo "Введите путь до файла"
	read -r pathToFile		
	eval "cat $pathToFile 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#[EN]Take name of file, then delete file /[RU]Запрашивает имя файла, после чего удаляет файл с указанным именем
deleteFile() {
	echo "Введите название файла"
	read -r pathToLink
	eval "rm $pathToLink 2>&1 | tee -a \"$SCRIPTLOGS\""
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
		2)printDirContent;;
		3)currentDatePrint;;
		4)printFile;;
		5)deleteFile;;
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