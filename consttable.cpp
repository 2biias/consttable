#include <iostream>

typedef enum
{
    baud_300,
    baud_600,
    baud_1200,
    baud_2400,
    baud_4800,
    baud_9600,
    baud_19200,
    baud_39400,
    numberOfBaudrates
} baudrateEnums_t;

constexpr baudrateEnums_t& operator++(baudrateEnums_t& baudrate)
{
switch(baudrate) {
case baud_300 : return baudrate = baud_600;
case baud_600 : return baudrate = baud_1200;
case baud_1200 : return baudrate = baud_2400;
case baud_2400 : return baudrate = baud_4800;
case baud_4800 : return baudrate = baud_9600;
case baud_9600 : return baudrate = baud_19200;
case baud_19200 : return baudrate = baud_39400;
case baud_39400 : return baudrate = baud_300;
case numberOfBaudrates : return baudrate = baud_300;
}
return baudrate; 
}

constexpr baudrateEnums_t operator++(baudrateEnums_t& baudrate, int)
{
  baudrateEnums_t tmp(baudrate);
  ++baudrate;
  return tmp;
}

struct basis_baudrate_consts
{
    const int bits_per_char = 11;
    const int fs = 192308;
    const int ADCvoltageRes_uV = 3300000/4095;
    const int slaveMarkThresh_uV = 21357;
    const int slaveSpaceThresh_uV = 32000;
    const int slaveMarkThresh_lsb = slaveMarkThresh_uV/ADCvoltageRes_uV;
    const int slaveSpaceThresh_lsb = slaveSpaceThresh_uV/ADCvoltageRes_uV;
    const int edgeThresh_lsb = (slaveSpaceThresh_lsb*2)/3;
};

struct baudrate_consts : public basis_baudrate_consts
{ 
    int baudrate = 0;
    int nominal_bittime = 0;
    int nominal_bittime_n = 0;
    int nominal_bittime_ms = 0;
    int nominal_chartime_n = 0;
    int min_startbittime_n = 0;
    int max_bittime_n = 0;
    int min_bittime_n = 0;
    int min_chartime_n = 0;
    int max_chartime_n = 0;
    int max_bittime_div_n = 0;
};

template<class T, int N>
struct Elements
{
    T elems[N];

    typedef T* iterator;
    typedef const T* const_iterator;
    constexpr iterator begin() { return &elems[0]; }
    constexpr const_iterator begin() const { return &elems[0]; }
    constexpr iterator end() { return &elems[N]; }
    constexpr const_iterator end() const { return &elems[N]; }

    constexpr T& operator[](int i)
    {
        return elems[i];
    }
    constexpr const T& operator[](int i) const
    {    
        return elems[i];
    }
};

constexpr Elements<struct baudrate_consts, numberOfBaudrates> make_table()
{
    Elements<struct baudrate_consts, numberOfBaudrates> baudrate_table = {};
    
    baudrateEnums_t baudrateEnum = baud_300;
    
    for( auto& baud : baudrate_table )
    {
        baud.baudrate = 300 * (1<<baudrateEnum);
        baud.nominal_bittime = 1/baud.baudrate;
        baud.nominal_bittime_n = baud.nominal_bittime*baud.fs;
        baud.nominal_bittime_ms = baud.nominal_bittime*1000;
        baud.nominal_chartime_n = baud.nominal_bittime_n*baud.bits_per_char;
        baud.min_startbittime_n = baud.nominal_bittime_n * 0.3;
        baud.max_bittime_n = baud.nominal_bittime_n * 1.4;
        baud.min_bittime_n = baud.nominal_bittime_n * 0.6;
        baud.min_chartime_n = baud.nominal_chartime_n - (baud.max_bittime_n - baud.nominal_bittime_n);
        baud.max_chartime_n = baud.nominal_chartime_n * 1.07;
        baud.max_bittime_div_n = baud.nominal_bittime_n - baud.min_bittime_n;
        baudrateEnum++;
    }
    
    return baudrate_table;
}

constexpr static Elements<struct baudrate_consts, numberOfBaudrates> baudrate_lookup = make_table();

int main()
{
    int var = baudrate_lookup[baud_300].baudrate;

    std::cout << "Baudrate: " << var <<  std::endl;
    std::cout << "Bits per char: " << baudrate_lookup[baud_300].bits_per_char << std::endl;
    for(auto& baud : baudrate_lookup)
    {
        std::cout << baud.baudrate << std::endl;
    }
    return 0;
}