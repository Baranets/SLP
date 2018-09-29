# SLP (Основы системного программирования)
Laboratory Works for System Language Programming (Shell, Bash, Perl, C)

## Лабораторная работа #1

Написать интерактивную программу (скрипт) на языке shell, которая выводит список действий с номерами и ожидает ввода номера пункта, после чего начинает выполнение заданных в этом пункте команд. Скрипт должен корректно обрабатывать ситуацию окончания входного потока данных и позволять работать с относительными и абсолютными именами файлов и каталогов, которые могут содержать: пробелы и символы табуляции; символы '*', '?', '[', ']', '-', '~', '$' или состоять только из них. 

Для команд удаления необходимо запрашивать собственное подтверждение дополнительно. Текст запроса должен соответствовать системному (выводимому командой rm). Ожидание ввода ответа происходит на новой строке. 

Организовать обработку ошибок, например, при отсутствии удаляемого файла, для чего перенаправить вывод системных сообщений об ошибках в файл-журнал с именем lab1_err, расположенном в домашней директории, и выдать свое сообщение на стандартный поток ошибок. 

Каждый запрос на ввод должен сопровождаться сообщением на отдельной строке, содержащим информацию о том, какие данные должны быть введены. Ожидание ввода происходит с новой строки. 

Выход из скрипта должен осуществляться только при выборе соответствующего пункта меню или окончании стандартного потока ввода. 

Каждый аргумент должен подаваться программе отдельной строкой. Например, нужно скопировать файл, тогда действия пользователя должны быть следующие:
1. Ввод числа, который соответствует пункту меню "Скопировать файл". Enter.
2. Ввод имени файла, который нужно скопировать. Enter.
3. Ввод пути, куда этот файл должен быть скопирован. Enter.

Меню должно выводиться при запуске скрипта и после каждого выполнения пункта. 

Исходный код лабораторной работы должен находиться в системе контроля версий, развёрнутой на helios. (`svn help`, `git help`).

### Вариант #1
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Напечатать содержимое текущего каталога
4. Создать файл
5. Удалить файл
6. Выйти из программы

### Вариант #2
1. Напечатать имя текущего каталога
2. Напечатать содержимое текущего каталога
3. Создать каталог
4. Сменить каталог
5. Удалить каталог
6. Выйти из программы

### Вариант #3
1. Напечатать имя текущего каталога
2. Создать файл
3. Отменить доступ к файлу для всех остальных пользователей
4. Отменить право на запись для владельца файла
5. Переименовать файл
6. Выйти из программы

### Вариант #4
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Выдать список пользователей, имеющих хотя бы один процесс
4. Создать файл
5. Скопировать файл
6. Выйти из программы

### Вариант #5
1. Напечатать имя текущего каталога
2. Напечатать содержимое текущего каталога
3. Вывести текущую дату и время в формате Wed Feb 2 01:01 MSK 2015
4. Вывести содержимое файла на экран
5. Удалить файл
6. Выйти из программы

### Вариант #6
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Напечатать содержимое текущего каталога
4. Создать прямую ссылку на файл
5. Удалить символическую ссылку на файл
6. Выйти из программы

### Вариант #7
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Выполнить введенную команду
4. Создать каталог
5. Удалить каталог вместе с его содержимым
6. Выйти из программы

### Вариант #8
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Создать файл
4. Предоставить всем право на запись в файл
5. Удалить файл
6. Выйти из программы

### Вариант #9
1. Напечатать имя текущего каталога
2. Напечатать содержимое текущего каталога
3. Создать каталог
4. Предоставить всем право на запись в каталог
5. Убрать всем право на запись в каталог
6. Выйти из программы

### Вариант #10
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Вывести текущую дату и время в формате Wed Feb 2 01:01 MSK 2015
4. Вывести содержимое файла на экран
5. Скопировать файл
6. Выйти из программы

### Вариант #11
1. Напечатать имя текущего каталога
2. Сменить текущий каталог
3. Напечатать содержимое текущего каталога
4. Создать косвенную (символьную) ссылку на файл
5. Выполнить введенную команду
6. Выйти из программы

### Вариант #12
1. Создать каталог
2. Сменить текущий каталог
3. Напечатать содержимое текущего каталога
4. Переместить файл
5. Удалить каталог с его содержимым
6. Выйти из программы


## Лабораторная работа #2

Написать два скрипта на языке shell, которые выводят списки:
### Вариант #1
- имён файлов в текущем каталоге, являющихся прямыми ссылками на указанный файл. Список отсортировать по времени доступа; 
- пользователей, принадлежащих к более чем указанному количеству групп.

### Вариант #2
- имён каталогов в указанном каталоге, имеющих не менее одного подкаталога. Список отсортировать по времени модификации;
- групп, которым принадлежат более чем указанное количество пользователей.

### Вариант #3
- имён файлов в текущем каталоге, являющихся косвенными ссылками на указанный файл. Список отсортировать по времени изменения метаинформации;
- пользователей принадлежащих указанной группе.

### Вариант #4
- исполняемых файлов, которые могут быть выполнены текущим пользователем без указания пути к ним. Список отсортировать по алфавиту в обратном порядке;
- пользователей, которым принадлежит процессов более указанного количества.

### Вариант #5
- имён процессов и их аргументов указанной группы;
- имён каталогов в указанном каталоге, содержащих файлы, но не имеющих подкаталогов.

### Вариант #6
- пользователей, заходивших в систему в течении текущего дня;
- файлов в указанном каталоге, имена которых состоят не только из букв латинского алфавита. Список отсортировать по размеру файла.

### Вариант #7
- пользователей, которым запрещено заходить в систему;
- файлов в указанном каталоге, являющихся pipe, открытых на запись каким-либо процессом.

Если не указано другого, все списки должны быть отсортированы по алфавиту. 
Вся необходимая информация должна передаваться в скрипты через аргументы командной строки.

## Лабораторная работа #3

Написать программу, выдающую:
1. Список пользователей, имеющих право записи в заданный файл.
2. Список пользователей, имеющих право чтения заданного файла.
3. Список пользователей, имеющих право исполнения заданного файла.
4. Список пользователей, имеющих право записи в заданный каталог.
5. Список пользователей, имеющих право чтения заданного каталога.
6. Список пользователей, имеющих право поиска в заданном каталоге.
7. Список файлов, для которых заданный пользователь имеет право записи.
8. Список файлов, для которых заданный пользователь имеет право чтения.
9. Список каталогов, в которыe заданный пользователь имеет право записи.
10. Список каталогов, в которыx заданный пользователь имеет право поиска.
11. Список файлов, для которых заданный пользователь имеет право исполнения.
12. Список каталогов, в которыx заданный пользователь имеет право удаления файлов.
13. Список файлов, содержимое которых можно только дополнять, но не заменять.

Вся необходимая информация должна передаваться в скрипт через аргументы командной строки.
Все файлы выбирать из текущего каталога исключая его подкаталоги.

Для защиты на 4 и более баллов скрипт должен:
1. проверять права по UID и GID, а не по именам
2. не использовать find/gfind/...

Для защиты на 4.5 и более баллов скрипт должен:
* учитывать ACL POSIX и NFSv4
