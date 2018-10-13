#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 6

#Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Сменить текущий католог" 
						"Напечатать содержание текущего католога" 
						"Создать прямую ссылку на файл" 
						"Удалить сиволическую ссылку на файл" 
						"Выйти из программы")

#Инициализируем переменную с расположением лог файлов
SCRIPTLOGS="$PWD/logs.txt"

#Создание лог файла в случее его отсутствия в каталоге со скриптом, иначе выводит сообщение о существовании такового файла
createLogFile() {
	if test -f "$SCRIPTLOGS"; then
		echo "Log file is exist"
	else
		touch "$SCRIPTLOGS"
	fi 
}

#Выводит в консоль "Главное меню"
printMenu() {
	for i in ${!systemCall[*]}
	do
		indexForUser=$[$i+1]
		echo $indexForUser"." ${systemCall[$i]}
	done
}

#Выводит в консоль "Текущий каталог"
printCurrentDir() {
	echo "Текущий каталог: "
	pwd
}

#Запрашивает путь к каталогу, затем переходит в указанный каталог
changeDir() {
	echo "Введите путь к каталогу"
	read -r pathToDir
	eval "cd $pathToDir 2>>\"$SCRIPTLOGS\" || echo Переход в каталог \"$pathToDir\" не возможен>&2"
}

#Выводит в консоль "Содержание текущего каталога"
printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

#Запрашивает имя файла, после чего создает файл с указанным именем
createLink() {
	echo "Введите путь до файла"
	read -r pathToFile
	echo "Введите имя для прямой ссылки"
	read -r pathToLink			
	eval "ln $pathToFile $pathToLink 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#Запрашивает имя файла, после чего удаляет файл с указанным именем
deleteLink() {
	echo "Введите название ссылки на файл"
	read -r pathToLink
	eval "unlink $pathToLink 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#Начало исполнения функциональной части скрипта
createLogFile

printMenu

while :
do
	read -n 1 key
	echo
	
	case $key in
		1)printCurrentDir;;
		2)changeDir;;
		3)printDirContent;;
		4)createLink;;
		5)deleteLink;;
		6)
			echo "Goodbye"
			break;;
		*)
			#Обработка события с неуказанным в case индексом с проверкой на ввод сочетания клавишь Ctrl-D
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
