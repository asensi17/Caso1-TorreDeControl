#pragma once

#include "Contexto.h"

class TorreControl;
class ServiciosTierra;

class Avion {
public:
    Avion(int id, const Contexto& ctx, TorreControl& torre, ServiciosTierra& servicios);

    void ejecutar();

private:
    int _id;
    const Contexto& _ctx;
    TorreControl& _torre;
    ServiciosTierra& _servicios;
    bool _interbloqueo;
};
