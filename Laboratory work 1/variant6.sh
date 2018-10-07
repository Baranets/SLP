#!/bin/bash
#Laboratory work 1; 
#Baranets

systemCall=("Напечатать имя текущего католога" 
						"Сменить текущий католог" 
						"Напечатать содержание текущего католога" 
						"Создать прямую ссылку на файл" 
						"Удалить сиволическую ссылку на файл" 
						"Выйти из программы")

createLogFile() {
	if test -f logs.txt; then
		echo "Log file is exist"
	else
		touch logs.txt
	fi 
}

printMenu() {
	for i in ${!systemCall[*]}
	do
		indexForUser=$[$i+1]
		echo $indexForUser"." ${systemCall[$i]}
	done
}

printCurrentDir() {
	echo "Текущий каталог: "
	pwd
}

changeDir() {
	echo "Введите путь к каталогу"
	read -r pathToDir
	cd "$pathToDir"
}

printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

createLink() {
	echo "Введите путь до файла"
	read -r pathToFile
	echo "Введите имя для прямой ссылки"
	read -r pathToLink			
	ln "$pathToFile" "$pathToLink" 2>&1 | tee -a logs.txt 
}

deleteLink() {
	echo "Введите название ссылки на файл"
	read -r pathToLink
	unlink "$pathToLink" 2>&1 | tee -a logs.txt
}

createLogFile

printMenu

while :
do
	read key
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
