#!/bin/bash
#Laboratory work 1; 
#Baranets 
#Variant 2

#Массив хранящий в себе наименования команд
systemCall=("Напечатать имя текущего католога" 
						"Напечатать содержание текущего католога" 
						"Создать каталог" 
						"Сменить текущий католог" 
						"Удалить каталог" 
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
	cd "$pathToDir" 2>>"$SCRIPTLOGS" || echo "Переход в каталог \"$pathToDir\" не возможен">&2
}

#Выводит в консоль "Содержание текущего каталога"
printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

#Запрашивает имя каталога, после чего создает каталог с указанным именем
createDir() {
	echo "Введите имя каталога"
	read -r nameFile		
	mkdir "$nameFile" 2>&1 | tee -a "$SCRIPTLOGS"
}

#Запрашивает имя каталога, после чего удаляет каталог с указанным именем
deleteDir() {
	echo "Введите имя каталога"
	read -r nameFile
	#Не удаляет не пустой каталог!
	rmdir "$nameFile" 2>&1 | tee -a "$SCRIPTLOGS"
}

#Начало исполнения функциональной части скрипта
createLogFile

printMenu

while :
do
	read key
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
