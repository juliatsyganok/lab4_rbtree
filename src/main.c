#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_menu() {
    printf("Команд:\n");
    printf("1 Добавить узел\n");
    printf("2 Удалить узел\n");
    printf("3 Печать дерева\n");
    printf("4 Поиск по ключу\n");
    printf("5 Выполнить команды из файла\n");
    printf("6 Всё\n");
    printf("Номер: ");
}

void process_file(RBTree* tree, const char* input_file, const char* output_file) {
    FILE* fin = fopen(input_file, "r");
    if (!fin) {
        printf("Ошибка открытия %s\n", input_file);
        return;
    }
    FILE* fout = fopen(output_file, "w");
    if (!fout) {
        printf("Ошибка создания %s\n", output_file);
        fclose(fin);
        return;
    }
    char line[256];
    int cmd;
    char key[7];
    double value;
    while (fgets(line, sizeof(line), fin)) {
        if (line[0] == '\n' || line[0] == '#')
            continue;
        fprintf(fout, "%s", line);
        if (sscanf(line, "%d", &cmd) != 1)
            continue;
        switch (cmd) {
            case 1: 
                if (sscanf(line, "%d %6s %lf", &cmd, key, &value) == 3) {
                    rbt_insert(tree, key, value);
                    fprintf(fout, "Добавлено: %s = %.2f\n", key, value);
                } else {
                    fprintf(fout, "Ошибка формата\n");
                }
                break;
            case 2: 
                if (sscanf(line, "%d %6s", &cmd, key) == 2) {
                    RBNode* found = rbt_search(tree, key);
                    if (found) {
                        rbt_delete(tree, key);
                        fprintf(fout, "Удалено: %s\n", key);
                    } else {
                        fprintf(fout, "Ключ %s не найден\n", key);
                    }
                } else {
                    fprintf(fout, "Ошибка формата\n");
                }
                break;
            case 3: 
                fprintf(fout, "Дерево:\n");
                rbt_print(tree, tree->root, 0);
                break;
            case 4: 
                if (sscanf(line, "%d %6s", &cmd, key) == 2) {
                    RBNode* found = rbt_search(tree, key);
                    if (found) {
                        fprintf(fout, "НАЙДЕН: %s = %.2f (%s)\n",
                                key, found->value,
                                found->color == RED ? "RED" : "BLACK");
                    } else {
                        fprintf(fout, "нет: %s\n", key);
                    }
                } else {
                    fprintf(fout, "Ошибка формата\n");
                }
                break;

            default:
                fprintf(fout, "Неизвестная команда: %d\n", cmd);
        }

        fprintf(fout, "\n");
    }
    fclose(fin);
    fclose(fout);
}

int main() {
    RBTree* tree = rbt_create();
    if (!tree) {
        printf("Ошибка создания дерева\n");
        return 1;
    }
    int choice;
    char key[7];
    double value;
    char filename[256];
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Ошибка ввода\n");
            continue;
        }
        getchar(); 
        switch (choice) {
            case 1:
                printf("Введите ключ и значение: ");
                if (scanf("%6s %lf", key, &value) == 2) {
                    rbt_insert(tree, key, value);
                    printf("✓ Добавлено: %s = %.2f\n", key, value);
                } else {
                    printf("Ошибка ввода\n");
                }
                getchar();
                break;
            case 2:
                printf("Введите ключ: ");
                if (scanf("%6s", key) == 1) {
                    RBNode* found = rbt_search(tree, key);
                    if (found) {
                        rbt_delete(tree, key);
                        printf("Удален: %s\n", key);
                    } else {
                        printf("нет\n");
                    }
                }
                getchar();
                break;
            case 3:
                printf("\nДерево:\n");
                if (tree->root != tree->NIL)
                    rbt_print(tree, tree->root, 0);
                else
                    printf("пусто\n");
                break;
            case 4:
                printf("Введите ключ: ");
                if (scanf("%6s", key) == 1) {
                    RBNode* found = rbt_search(tree, key);
                    if (found) {
                        printf("✓ Найден: %s = %.2f (%s)\n",
                               key, found->value,
                               found->color == RED ? "КРАСНЫЙ" : "ЧЁРНЫЙ");
                    } else {
                        printf("нет\n");
                    }
                }
                getchar();
                break;

            case 5:
                printf("имя файла: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                process_file(tree, filename, "output.txt");
                break;
            case 6:
                rbt_free(tree);
                return 0;
            default:
                printf("Ошибка\n");
        }
    }
}