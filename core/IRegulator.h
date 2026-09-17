#pragma once

class IRegulator {
    public:
        virtual ~IRegulator() = default;

        virtual double symuluj(double uchyb) = 0;
        virtual void resetPamieci() = 0;
};
