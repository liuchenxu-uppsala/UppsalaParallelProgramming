//
// Created by Chenx on 2025/10/14.
//

#ifndef PTHREADPI_PTHREADREADWRITELOCK_H
#define PTHREADPI_PTHREADREADWRITELOCK_H
void init_reader_writer_lock();
void read_lock();
void write_lock();
void read_write_unlock();
#endif //PTHREADPI_PTHREADREADWRITELOCK_H