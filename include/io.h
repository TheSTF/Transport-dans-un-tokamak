
#ifndef TOKAMAK_IO_H
#define TOKAMAK_IO_H

#include "grid.h"

int io_write_csv(const char *path, const Field *f);
int io_write_vtk(const char *path, const Field *f);

#endif /* TOKAMAK_IO_H */