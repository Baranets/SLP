#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 4

#Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Сменить текущий каталог" 
						"Выдать список пользователей, имеющих хотя бы один процесс" 
						"Создать файл" 
						"Скопировать файл" 
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

#Запрашивает путь к каталогу, затем переходит в указанный каталог
changeDir() {
	echo "Введите путь к каталогу"
	read -r pathToDir
	cd "$pathToDir" 2>>"$SCRIPTLOGS" || echo "Переход в каталог \"$pathToDir\" не возможен">&2
}

#Выводит в консоль пользователей имеющих хотя бы один процесс
showUsersProcess() {
	echo "Пользователи имеющие процессы: "
	ps -eo user | grep -v "USER" | sort | uniq
}

#Создает файл с указанным именем
createFile() {
	echo "Введите имя файла"
	read -r nameFile
	touch "$nameFile" 2>>"$SCRIPTLOGS" || echo "Неудалось создать файл \"$nameFile\"">&2
}

#Удаляет право владельца файла на чтение
copyFile() {
	echo "Введите имя копируемого файла"
	read -r nameFile
	echo "Введите целевой каталог"
	read -r destinationPath
	cp "$nameFile" "$destinationPath" 2>&1 | tee -a "$SCRIPTLOGS"
}

#Начало исполнения функциональной части скрипта
createLogFile

printMenu

while :
do
	read key
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
			#Обработка события с неуказанным в case индексом с проверкой на ввод сочетания клавишь Ctrl-D
    		line="$key"
    		printf -v key_code "%d" "'$key"
    		if [ $key_code -eq 0 ]; then
        		echo "Ctrl-D LOOL! Goodbye!"
        		break
        	else 
        		echo "Неверный индекс"
        	fi
        	;;
	esac
done
