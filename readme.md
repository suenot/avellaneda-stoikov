# Market Maker Project

Проект реализует алгоритм маркет-мейкинга для пар USD+/wETH и USD+/cbbtc на основе модели Avellaneda-Stoikov с учетом специфики onchain-торговли.

## Сборка
1. Установите CMake и компилятор C++ (g++, clang++).
2. Создайте директорию `build`:
   ```bash
   mkdir build && cd build
   ```
3. Сгенерируйте файлы сборки:
   ```bash
   cmake ..
   ```
4. Скомпилируйте проект:
   ```bash
   make
   ```
5. Запустите:
   ```bash
   ./market_maker
   ```

## Текущая реализация
- Базовая модель Avellaneda-Stoikov для расчета спредов.
- Адаптация под onchain: учет latency и gas costs.
- Управление инвентарем.
- Заглушки вместо данных Binance и onchain-метрик.

## Доработка
- Подключите Binance API (Boost или libcurl).
- Интегрируйте web3cpp для onchain-данных.
- Добавьте RL (например, LibTorch для PPO).
