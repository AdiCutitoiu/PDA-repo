#ifndef _MPI_DATA_TYPE_H_
#define _MPI_DATA_TYPE_H_

namespace MPI
{
  namespace Detail
  {
    template<typename T>
    struct MPIDataType;

    template<>
    struct MPIDataType<char>
    {
      static const int ID = MPI_CHAR;
    };

    template<>
    struct MPIDataType<unsigned char>
    {
      static const int ID = MPI_UNSIGNED_CHAR;
    };

    template<>
    struct MPIDataType<wchar_t>
    {
      static const int ID = MPI_WCHAR;
    };

    template<>
    struct MPIDataType<short>
    {
      static const int ID = MPI_SHORT;
    };

    template<>
    struct MPIDataType<unsigned short>
    {
      static const int ID = MPI_UNSIGNED_SHORT;
    };

    template<>
    struct MPIDataType<int>
    {
      static const int ID = MPI_INT;
    };

    template<>
    struct MPIDataType<unsigned int>
    {
      static const int ID = MPI_UNSIGNED;
    };

    template<>
    struct MPIDataType<long int>
    {
      static const int ID = MPI_LONG;
    };

    template<>
    struct MPIDataType<unsigned long int>
    {
      static const int ID = MPI_UNSIGNED_LONG;
    };

    template<>
    struct MPIDataType<long long int>
    {
      static const int ID = MPI_LONG_LONG_INT;
    };

    template<>
    struct MPIDataType<float>
    {
      static const int ID = MPI_FLOAT;
    };

    template<>
    struct MPIDataType<double>
    {
      static const int ID = MPI_DOUBLE;
    };

    template<>
    struct MPIDataType<long double>
    {
      static const int ID = MPI_LONG_DOUBLE;
    };
  }
}

#endif // !_MPI_DATA_TYPE_H_
