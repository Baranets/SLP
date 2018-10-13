#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 3

#Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Создать файл" 
						"Отменить доступ к файлу для всех остальных пользователей" 
						"Отменить право на запись для владельца файла" 
						"Переименовать файл" 
						"Выйти из программы")

#Инициализируем переменную с расположением лог файлов
SCRIPTLOGS="$PWD/logs.txt"
echo $SCRIPTLOGS

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

#Создает файл с указанным именем
createFile() {
	echo "Введите имя файла"
	read -r nameFile
	eval "touch $nameFile 2>>\"$SCRIPTLOGS\" || echo Неудалось создать файл \"$nameFile\">&2"
}

#Удаляет все права на файл у остальных пользователей
deleteRootsOtherUsers() {
	echo "Введите имя файла"
	read -r nameFile
	eval "chmod o-rwx $nameFile 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#Удаляет право владельца файла на чтение
deleteWriteRootUser() {
	echo "Введите имя файла"
	read -r nameFile
	eval "chmod u-w $nameFile 2>&1 | tee -a \"$SCRIPTLOGS\""
}

#Переименовывает указанный файл
renameFile() {
	echo "Введите текущее имя файла"
	read -r oldNameFile
	echo "Введите новое имя файла"
	read -r newNameFile
	eval "mv $oldNameFile $newNameFile 2>&1 | tee -a \"$SCRIPTLOGS\""
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
		2)createFile;;
		3)deleteRootsOtherUsers;;
		4)deleteWriteRootUser;;
		5)renameFile;;
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
    		fi
        	;;
	esac
done
