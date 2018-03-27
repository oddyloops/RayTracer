using System.Threading.Tasks;
using System.Collections.Generic;

namespace RTMeld
{
    public interface IDataContext
    {
        void Connect();

        int Insert<T>(T data);

        Task<int> InsertAsync<T>(T data);

        IEnumerable<T> SelectAll<T>();

        T SelectOne<T, K>(K key);

        Task<T> SelectOneAsync<T, K>(K key);

        List<T> SelectMatching<T>(T match
            )




    }
}
