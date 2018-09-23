#!/bin/sh
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
	read pathToDir
	cd $pathToDir 2>&1 | tee -a logs.txt
}

printDirContent() {
	echo "Содержание текущего каталога"
	ls
}

createLink() {
	echo "Введите путь до файла"
	read pathToFile
	echo "Введите имя для прямой ссылки"
	read pathToLink			
	ln $pathToFile $pathToLink 2>&1 | tee -a logs.txt 
}

deleteLink() {
	echo "Введите название ссылки на файл"
	read pathToLink
	rm -r $pathToLink 2>&1 | tee -a logs.txt
}

createLogFile

printMenu

while :
do
	read inputIndex
	case $inputIndex in
		1)printCurrentDir;;
		2)changeDir;;
		3)printDirContent;;
		4)createLink;;
		5)deleteLink;;
		6)
			echo "Goodbye"
			break;;
		*)echo "Неверный индекс";;	
	esac
done
