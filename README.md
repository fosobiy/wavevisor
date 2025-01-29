# Пример разработки приложения под KolibriOS

В примере использовались:
- KolibriOS Tiny C Compiler
- PyCDLib
- TinyGL для KolibriOS
- Может ещё что-то, если я что-то забыл

Как собрать?
1. Поставить Python 3.11.6
2. Запустить (нажать двойным кликом) build.bat

Как запусть пример?
1. Поставить Virtual Box
2. Создать в Virtual Box машину не создавая диск
3. Добавить в виртуалку один флопповод и один дисковод
4. В флопповод загрузить образ KolibriOS (kolibri.img)
5. В дисковод загрузить "examplec.iso"
6. Запустить виртуалку
7. В запущенной ОС тыкнуть на иконку "KFM2"
8. Над левым полуокном выбрать cd0/1 (в меню, где написано "sys", выпадающий список будет)
9. В левом полуокне будет один единственный каталог. Жмём на него, и там будет программа, её и запускаем.
