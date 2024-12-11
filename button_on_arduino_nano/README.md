# Аркадные кнопки на ардуино

В данном проекта представленны два скретча которые необходимо загрудить на 2 разные платы ардуино

## Комплектующие
Плата arduino nano
Модуль связи nRF24L01.h
Любоя аркадная кнопка без фиксатора
Один или два светодиода для индикации
Копрус

Данный комплект для одной кнопки.

## Алгоритм работы
Данные кнопки устанавливают радиосвязь бежду собой.

Если она из кнопок (Кнопка А) нажата, она отпавляет сигнал второй кнопке (кнопка Б), сапа при этом мигает светодиодом в течении 5 секунд.

Если надата другая кнопка (кнопка Б), то вторая кнопка (кнопка А) блокируется на то же время.

Данный процесс одиноков для обеих кнпок.

Два файла нужны, так как у них написаны разные каналы связи для приема и передачи сообщений о нажатии кнопки.