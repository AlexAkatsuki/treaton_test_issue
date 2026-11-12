#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/** Для своего удобства разделил задачки через опцию cmake - TASK **/

// ******************** ЗАДАЧА 1 *************************//
#ifdef TASK_1
std::string first_unique(const std::vector<std::string>& arr) {
  std::unordered_map<std::string_view, std::size_t> cnt;

  /** (*2)микрооптимизация: снижаем фактор загрузки, жертвуя памятью.
   * вообще не обязательно, но если важны микросекунды то почему бы и да)**/
  cnt.reserve(arr.size() * 2);

  for(const auto& w : arr)
    ++cnt[w];

  /** тут тоже можно добавить микрооптимизация для скорости, заменив if и for
   * на while с двумя условиями, сделав постпроверку в return тернарником **/
  for(const auto& w : arr) {
    if(cnt[w] == 1)
      return w;
  }

  return "";
}

int main() {
  std::vector<std::string> arr = {
    "Alisa",
    "Vitya",
    "Rita",
    "Alisa",
    "Vitya",
    "Rusya",
    "Elizaveta Nikitichna",
    "Aleksandr Alekseevich",
    "Rita",
  };

  std::string str = first_unique(arr);

  if(str.empty()) {
    std::cout << "ups\n";

    return 0;
  }

  std::cout << str << "\n";
}
#endif

// ******************** ЗАДАЧА 2 *************************//
#ifdef TASK_2

/** Не хватало инклюдов **/
#include <cctype> /** тайп для  tolower **/
#include <memory> /** умный указатель **/

struct Worker {
  std::string name;

  std::function<void()> onDone;

  /** для избежения лишних выделений памяти можно добавить : name{std::move(n)}
   * то есть привести к &&ссылке, если проще то name сразу же получит своё
   * значение через память n, без лишних перемещений
   *
   * А вообще, если нет необходимости передавать в конструктор параметры таким
   * образом, то проще было бы просто сделать вот так, после того как появится
   * конструктор по умолчанию:
   *
   * auto worker = std::make_shared<Worker>();
   * worker->name = "IDLE";
   * std::cout << "Worker name: " << worker->getNameInLowerCase() << std::endl;
   *
   * И у меня есть подозрение что воркер - это некий стэйтмашин, в таком случае
   * есть вероятность что его стоит сделать статичным
   *
   * **/
  explicit Worker(std::string n) : name{n} {}

  /** 1. возврат ссылки на локальный объект std::string& (на след шаге его
   * уже не проект просто не соберётся будет), тут вероятнее всего будет падать
   * , да и настроенный проект просто не соберётся простейшее решение -
   * возвращать значение
   *
   * 2. tolower не воспринмает нормально кириллицу из
   * за разной байтовой кодировки, будут иероглифы в консоли
   * Решений масса, зависит от того к какой ОСИ мы привязаны, есть windows api,
   * есть универсальные библиотеки(правда придётся тянуть зависимость, а надо ли
   * оно нам)
   * Конкретных способов описать не рискну, так как в реальной практике не
   * использовал, но лично я был бы за кроссплатформленное решение **/
  std::string& getNameInLowerCase() {
    std::string lc{name};
    std::transform(lc.begin(), lc.end(), lc.begin(), [](unsigned char c) {
      /** tolower возвращает int лучше добавить static cast к char **/
      return std::tolower(c);
    });
    return lc;
  }
};

std::shared_ptr<Worker> createWorker(std::string name) {
  auto worker{std::make_shared<Worker>(name)};

  /** Передаём значение - плохо, ссылка на worker, останется
   * 1-я ссылка в мэйне, 2-я локальная в createWorker, и её мы передаём через
   * лямбду обратно в worker крч, тут утечка памяти
   *
   * Вариант 1: слабый указатель, потому что не инкрементируект счётчик
   * Вариант2: передавать только name
   * Вариант 3: избавиться от std::function, и вместо него просто сделать метод
   * класса **/
  worker->onDone = [worker]() {
    std::cout << "Task " << worker->name << " completed.\n";
  };

  return worker;
}

int main() {
  auto worker{createWorker("IDLE")};

  /** вместо лишний std::endl вызовов лучше как в createWorker оставить c-шный
   * перенос строки. Не критично, но быстрее**/
  std::cout << "Worker name: " << worker->getNameInLowerCase() << std::endl;

  worker->onDone();

  return 0; /**не обязателен, тут дело вкуса **/
}

#endif
