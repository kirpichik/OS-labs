//
// Lab work #6, Multithread cp -R
//

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>

#define REQUEST_FD_TIMEOUT 1
#define BUFFER_SIZE 1024

typedef struct copy_task {
  size_t src_len;
  size_t dst_len;
  mode_t mode;
  char* source_path;
  char* dest_path;
} copy_task_t;

char* append_path(char*, char*);

copy_task_t* create_copy_task(char*, char*, mode_t);
void free_copy_task(copy_task_t*);

int request_fd(char*, int, mode_t*);
int handle_path(char*, char*);

void* copy_file_thread(void*);
void* scan_dir_thread(void*);

/**
 * Присодениняет к названию родительского каталога его дочерний объект.
 *
 * @param parent Родительский каталог.
 * @param child Дочерний объект.
 *
 * @return Новая строка на куче в виде: parent + '/' + child
 */
char* append_path(char* parent, char* child) {
  size_t parent_len = strlen(parent);
  size_t child_len = strlen(child);

  char* result = (char*) malloc(parent_len + child_len + 2);
  if (!result)
    return NULL;

  memcpy(result, parent, parent_len);
  result[parent_len] = '/';
  strcpy(result + parent_len + 1, child);

  return result;
}

/**
 * Создает задачу для копирования.
 *
 * @param src Основной исходный путь.
 * @param dst Основной целевой путь.
 * @param mode Права доступа.
 *
 * @return Новая задача, память под которую выделена на куче.
 */
copy_task_t* create_copy_task(char* src, char* dst, mode_t mode) {
  copy_task_t* task = (copy_task_t*) malloc(sizeof(copy_task_t));
  if (!task)
    return NULL;

  task->src_len = strlen(src);
  task->dst_len = strlen(dst);
  task->source_path = strdup(src);
  task->mode = mode;
  if (!task->source_path) {
    free(task);
    return NULL;
  }
  task->dest_path = strdup(dst);
  if (!task->dest_path) {
    free(task->source_path);
    free(task);
    return NULL;
  }

  return task;
}

/**
 * Освобождает память задачи копирования.
 *
 * @param task Освобождаемая задача.
 */
void free_copy_task(copy_task_t* task) {
  free(task->source_path);
  free(task->dest_path);
  free(task);
}

/**
 * Запрашивает файловый дескриптор для файла с указанными флагами.
 * Если на данный момент нет доступных файловых дескрипторов,
 * ждет таймаут и перезапрашивает его. Так до тех пор, пока
 * дескриптор не будет выдан или же не произайдет иная ошибка.
 *
 * @param pathname Путь до файла.
 * @param flags Флаги для дескриптора.
 * @param mode Права доступа. Требуются только для создания файла, иначе NULL.
 *
 * @return Дескриптор или -1, если произошла ошибка.
 */
int request_fd(char* pathname, int flags, mode_t* mode) {
  int fd;
  while ((fd = !mode ? open(pathname, flags) : open(pathname, flags, *mode)) == -1) {
    if (errno != EMFILE) {
      perror("Cannot open file");
      return -1;
    }

    sleep(REQUEST_FD_TIMEOUT);
  }

  return fd;
}

/**
 * Обрабатывает путь файловой системы и решает, что с ним нужно сделать.
 * Для файла вызывает поток копирования файла
 * Для директории вызывает потом сканирования директории
 * Для всего остального ничего не вызывает.
 *
 * @param src_path Исходный путь.
 * @param dst_path Целевой путь.
 *
 * @return 0 если путь обработан, -1 если произошла ошибка,
 * 1 если путь проигнорирован.
 */
int handle_path(char* src_path, char* dst_path) {
  struct stat src_pathstat;
  pthread_t thread;
  copy_task_t* task;

  if (stat(src_path, &src_pathstat) == -1) {
    perror("Cannot get path stat");
    return -1;
  }

  if (S_ISREG(src_pathstat.st_mode) || S_ISDIR(src_pathstat.st_mode)) {
    if (!(task = create_copy_task(src_path, dst_path, src_pathstat.st_mode)))
      return -1;

    void* (*func)(void*) = S_ISREG(src_pathstat.st_mode) ? &copy_file_thread : &scan_dir_thread;
    if (pthread_create(&thread, NULL, func, task) == -1) {
      perror("Cannot create thread");
      free_copy_task(task);
      return -1;
    }

    return 0;
  }

  return 1;
}

/**
 * Функция потока копирующего файл.
 */
void* copy_file_thread(void* target) {
  copy_task_t* task = (copy_task_t*) target;
  int src_fd = request_fd(task->source_path, O_RDONLY | O_CREAT | O_EXCL, NULL);
  if (src_fd == -1)
    return NULL;
  int dst_fd = request_fd(task->dest_path, O_WRONLY, &task->mode);
  if (dst_fd == -1)
    return NULL;

  char* buff[BUFFER_SIZE];
  ssize_t count;

  while ((count = read(src_fd, buff, BUFFER_SIZE)) > 0) {
    if (write(dst_fd, buff, count) == -1) {
      perror("Cannot write file");
      close(src_fd);
      close(dst_fd);
      free_copy_task(task);
      return NULL;
    }
  }

  if (count == -1)
    perror("Cannot read file");

  close(src_fd);
  close(dst_fd);
  free_copy_task(task);
  return NULL;
}


/**
 * Функция потока сканирующего директорию.
 */
void* scan_dir_thread(void* target) {
  copy_task_t* task = (copy_task_t*) target;
  DIR* dir;
  int ret;
  struct dirent* result;
  struct dirent* entry;
  long name_max;

  if (mkdir(task->dest_path, task->mode) == -1) {
    perror("Cannot create target dir");
    free_copy_task(task);
    return NULL;
  }

  if ((name_max = pathconf(task->dest_path, _PC_NAME_MAX)) == -1) {
    perror("Cannot get path attributes");
    return NULL;
  }

  // Выделение чуть больше размера структуры из-за записи имени файла в конце
  entry = (struct dirent*) malloc(sizeof(struct dirent) + name_max + 1);

  if ((dir = opendir(task->source_path) == NULL)) {
    perror("Cannot read dir structure");
    free_copy_task(task);
    return NULL;
  }

  while ((ret = readdir_t(dir, entry, &result)) == 0 && result != NULL) {
    char* src = append_path(task->source_path, entry->d_name);
    char* dst = append_path(task->dest_path, entry->d_name);

    int handle_res = handle_path(src, dst);
    free(src);
    free(dst);
    if (handle_res == -1) {
      free(entry);
      closedir(dir);
      free_copy_task(task);
      return NULL;
    }
  }

  if (ret != 0)
    perror("Cannot read dir structure");

  free(entry);
  closedir(dir);
  free_copy_task(task);
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <src_path> <dst_path>\n", argv[0]);
    return -1;
  }

  struct stat dst_stat;
  int result = stat(argv[2], &dst_stat);
  if (result == 0) {
    printf("Destination path already exists.\n");
    return -1;
  } else if (result == -1 && errno != ENOENT) {
    perror("Wrong destination path");
    return -1;
  }

  result = handle_path(argv[1], argv[2]);
  if (result == 1) {
    printf("Source path is not file or dir.\n");
  }

  return result;
}

