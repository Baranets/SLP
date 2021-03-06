#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 2

#[EN]Array with names of functionality /[RU]Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Напечатать содержание текущего католога" 
						"Создать каталог" 
						"Сменить текущий католог" 
						"Удалить каталог" 
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

#[EN]Go to Directory /[RU]Запрашивает путь к каталогу, затем переходит в указанный каталог
changeDir() {
	echo "Введите путь к каталогу"
	read -r pathToDir
	eval "cd $pathToDir 2>>\"$SCRIPTLOGS\" || echo Переход в каталог \"$pathToDir\" не возможен>&2"
}

#[EN]Print to console "Current directory content" /[RU]Выводит в консоль "Содержание текущего каталога"
printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

#[EN]Take name of directory, then create directory /[RU]Запрашивает имя директории, после чего создает директорию с указанным именем
createDir() {
	echo "Введите имя каталога"
	read -r nameFile		
	eval "mkdir $nameFile 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#[EN]Take name of directory, then create directory /[RU]Запрашивает имя директории, после чего создает директорию с указанным именем
deleteDir() {
	echo "Введите имя каталога"
	read -r nameFile
	#Не удаляет не пустой каталог!
	eval "rmdir $nameFile 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#[EN]The begin of the functional part of the script /[RU]Начало исполнения функциональной части скрипта
createLogFile

printMenu

while :
do
	read -n 1 key
	case $key in
		1)printCurrentDir;;
		2)printDirContent;;
		3)createDir;;
		4)changeDir;;
		5)deleteDir;;
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
    		fi
        	;;
	esac
done
