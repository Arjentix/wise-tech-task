# Задание для ВАИС Техники

## Сборка

```bash
git clone 
git submodule init
git submodule update
cmake -Bbuild . && cmake --build build --target install
```

# Запуск

```bash
bin/wise_tech_task <filename>
```

Если файл содержит много данных, то лучше использовать

```bash
bin/wise_tech_task <filename> > out.txt
```

Файл  с входными данными содержится в папке *resources*, так что можно запускать

```bash
bin/wise_tech_task resources/phase_task.txt > out.txt
```

