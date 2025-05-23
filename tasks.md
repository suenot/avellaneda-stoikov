Вот подробное техническое задание (ТЗ) для реализации проекта маркет-мейкинга USD+/wETH и USD+/cbbtc с использованием модифицированной модели Avellaneda-Stoikov (A-S) и Reinforcement Learning (PPO). Я разобью задачу на небольшие подзадачи с чекбоксами в формате Markdown, чтобы даже начинающий разработчик (джун) мог взять отдельную задачу, понять контекст и выполнить её. Каждой задаче будет сопутствовать краткое описание и необходимые зависимости.

---

## Техническое задание (ТЗ): Реализация алгоритма маркет-мейкинга для USD+/wETH и USD+/cbbtc

### Общее описание проекта
Цель: Создать алгоритм маркет-мейкинга для onchain-торговли (1inch PMM, Hashflow PMM) с использованием данных CEX (Binance) и адаптацией под специфику DeFi (latency, gas costs). Базовая модель — Avellaneda-Stoikov, дополненная RL-алгоритмом PPO для динамической оптимизации спредов и объемов ордеров.

### Требования
- Язык программирования: c++
- Источники данных: Binance API, onchain-данные (etherscan, Infura или Alchemy).
- Окружение: Локальная симуляция + возможность подключения к тестнету (например, Sepolia).

### Общий контекст
- **Пары:** USD+/wETH, USD+/cbbtc.
- **Модель A-S:** Определяет базовые спреды на основе цены, инвентаря и волатильности.
- **PPO:** Адаптирует спреды и объемы ордеров в реальном времени.
- **Onchain-особенности:** Учет задержек (latency), gas costs и AMM-механик.

---

## Список задач с чекбоксами

### 1. Подготовка окружения и данных
#### Задача 1.1: Настройка окружения
- [x]

#### Задача 1.2: Подключение к Binance API
- [x] Реализовать функцию для получения текущей цены (\( S_t \)) и глубины книги ордеров для пар USD+/wETH и USD+/cbbtc через Binance API.
- **Описание:** Использовать публичный REST API Binance (например, endpoint `/api/v3/ticker/bookTicker`).
- **Результат:** Функция `get_binance_data(pair)` возвращает словарь с `mid_price`, `bid`, `ask`, `volume`.
- **Зависимости:** Документация Binance API, библиотека `requests`.

#### Задача 1.3: Подключение к onchain-данным
- [ ] Написать функцию для получения текущего gas price и времени подтверждения блока через провайдер (Infura/Alchemy).
- **Описание:** Использовать `web3.py` для запроса `eth_gasPrice` и средней задержки (по последним блокам).
- **Результат:** Функция `get_onchain_metrics()` возвращает `gas_price` (в wei) и `latency` (в секундах).
- **Зависимости:** `web3.py`, ключ API провайдера.

#### Задача 1.4: Сбор исторических данных для симуляции
- [ ] Скачать исторические данные цен и объемов для USD+/wETH и USD+/cbbtc с Binance (1-минутные свечи за 7 дней).
- **Описание:** Использовать Binance API или готовые библиотеки.
- **Результат:** CSV-файл с колонками: `timestamp`, `open`, `high`, `low`, `close`, `volume`.
- **Зависимости:** Задача 1.2.

---

### 2. Реализация базовой модели Avellaneda-Stoikov
#### Задача 2.1: Расчет волатильности (\( \sigma \))
- [x] Реализовать функцию для вычисления волатильности на основе исторических данных.
- **Описание:** Использовать стандартное отклонение логарифмических доходностей за последние 5 минут.
- **Результат:** Функция `calculate_volatility(data, window=5)` возвращает \( \sigma \) в процентах.
- **Зависимости:** `pandas`, `numpy`, данные из задачи 1.4.

#### Задача 2.2: Оценка интенсивности ордеров (\( k \))
- [x] Написать функцию для оценки \( k \) на основе глубины книги ордеров Binance.
- **Описание:** \( k = \frac{\text{объем ордеров}}{\text{ширина спреда}} \) в заданном диапазоне (например, ±1% от mid-price).
- **Результат:** Функция `estimate_order_intensity(book_data)` возвращает \( k \).
- **Зависимости:** Задача 1.2.

#### Задача 2.3: Реализация базовых спредов A-S
- [x] Реализовать формулы A-S для вычисления \( \delta_a \) и \( \delta_b \).
- **Описание:** Использовать уравнения:
  \[
  \delta_a = S_t + \frac{1}{\gamma} \ln(1 + \frac{\gamma}{k}) + q_t \cdot \sigma^2 T
  \]
  \[
  \delta_b = S_t - \frac{1}{\gamma} \ln(1 + \frac{\gamma}{k}) - q_t \cdot \sigma^2 T
  \]
  где \( \gamma = 0.1 \), \( T = 300 \) секунд (по умолчанию).
- **Результат:** Функция `calculate_spreads(S_t, q_t, sigma, k, gamma, T)` возвращает \( \delta_a, \delta_b \).
- **Зависимости:** Задачи 2.1, 2.2.

#### Задача 2.4: Управление инвентарем (\( q_t \))
- [x] Создать класс `InventoryManager` для отслеживания текущего инвентаря.
- **Описание:** Методы: `update_inventory(trade_size, side)` (покупка/продажа), `get_inventory()`.
- **Результат:** \( q_t \) обновляется после каждой симулированной сделки.
- **Зависимости:** Нет.

---

### 3. Адаптация под onchain-торговлю
#### Задача 3.1: Учет gas costs
- [x] Добавить расчет затрат на gas в модель A-S.
- **Описание:** Gas cost = `gas_price` × `gas_limit` (например, 100,000 gas для размещения ордера).
- **Результат:** Функция `calculate_gas_cost(gas_price)` возвращает стоимость в ETH.
- **Зависимости:** Задача 1.3.

#### Задача 3.2: Учет latency
- [x] Модифицировать \( S_t \) с учетом задержки.
- **Описание:** \( S_{t+\Delta t} = S_t + \epsilon \), где \( \epsilon \sim N(0, \sigma \sqrt{\Delta t}) \).
- **Результат:** Функция `adjust_price_with_latency(S_t, sigma, latency)` возвращает скорректированную цену.
- **Зависимости:** Задачи 1.3, 2.1.

#### Задача 3.3: Интеграция с PMM-пулами
- [x] Написать функцию для расчета спредов с учетом кривой ликвидности PMM.
- **Описание:** Использовать упрощенную модель \( \text{Price} = S_t \cdot (1 + \delta) \) с корректировкой на \( L_t \) (глубина пула).
- **Результат:** Функция `adjust_spreads_for_pmm(S_t, delta_a, delta_b, pool_depth)` возвращает адаптированные спреды.
- **Зависимости:** Задача 2.3, данные о пуле (заглушка или onchain).

---

### 4. Реализация RL (PPO)
#### Задача 4.1: Определение состояния и действий
- [x] Создать класс `MarketMakingEnv` для OpenAI Gym с состоянием и действиями.
- **Описание:**
  - State: \( (S_t, q_t, \sigma_t, \Delta t, L_t) \).
  - Action: \( (\delta_a, \delta_b, V_a, V_b) \) в диапазоне [0.001, 0.05] для спредов и [0.1, 10] для объемов.
- **Результат:** Рабочая среда с методами `step()` и `reset()`.
- **Зависимости:** `gym`, данные из задач 1.2, 1.3, 2.1.

#### Задача 4.2: Определение награды
- [x] Реализовать функцию награды.
- **Описание:** \( R_t = \text{Profit}_t - \gamma \cdot (\text{Inventory Risk})_t - \text{Gas Cost}_t \).
- **Результат:** Метод `calculate_reward(profit, q_t, gas_cost)` в `MarketMakingEnv`.
- **Зависимости:** Задачи 2.4, 3.1.

#### Задача 4.3: Обучение PPO
- [x] Настроить и запустить обучение PPO на симулированных данных.
- **Описание:** Использовать 10,000 итераций.
- **Результат:** Обученная модель PPO, сохраненная в файл.
- **Зависимости:** Задачи 4.1, 4.2, данные из 1.4.

---

### 5. Тестирование и визуализация
#### Задача 5.1: Симуляция маркет-мейкинга
- [x] Написать скрипт для симуляции торговли с использованием A-S и PPO.
- **Описание:** Использовать исторические данные для имитации сделок.
- **Результат:** Лог с прибылью, инвентарем и спредами.
- **Зависимости:** Все предыдущие задачи.

#### Задача 5.2: Визуализация результатов
- [ ] Построить графики: прибыль, инвентарь, спреды.
- **Описание:** Использовать `nextjs+nightvision`.
- **Результат:** Отдельная папка с frontend + rest сервис, которые отдает данные для визуализации
- **Зависимости:** Задача 5.1.

---

## Пример последовательности выполнения для джуна
1. Взять задачу 1.1, установить окружение.
2. Перейти к 1.2, подключиться к Binance API, протестировать на паре USD+/wETH.
3. Постепенно двигаться дальше, используя зависимости.
