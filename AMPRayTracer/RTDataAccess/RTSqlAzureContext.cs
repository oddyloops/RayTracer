using System;
using System.Collections.Generic;
using System.Linq.Expressions;

namespace RTDataAccess
{
    public class RTSqlAzureContext : DataContext
    {
        public override void Connect()
        {
            throw new NotImplementedException();
        }

        public override void Connect(string str)
        {
            throw new NotImplementedException();
        }

        public override void Commit()
        {
            throw new NotImplementedException();
        }

        public override void RollBack()
        {
            throw new NotImplementedException();
        }

        public override int Delete<K>(K key, Type type)
        {
            throw new NotImplementedException();
        }

        public override int DeleteMatching<T>(Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }

        public override int ExecuteNonQuery(string exec, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override int ExecuteNonQuery<T>(string exec)
        {
            throw new NotImplementedException();
        }

        public override int Insert<T>(T data)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<IDictionary<string, object>> Query(string query, IDictionary<string, object> paramMap)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> Query<T>(string exec)
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectAll<T>()
        {
            throw new NotImplementedException();
        }

        public override IEnumerable<T> SelectMatching<T>(Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }

        public override T SelectOne<T, K>(K key)
        {
            throw new NotImplementedException();
        }

        public override IList<T> SelectRange<T>(Expression<Func<T, bool>> matcher, int from, int length)
        {
            throw new NotImplementedException();
        }

        public override int Update<K, T>(K key, T newData)
        {
            throw new NotImplementedException();
        }

        public override int UpdateMatching<T>(T newData, Expression<Func<T, bool>> matcher)
        {
            throw new NotImplementedException();
        }
    }
}
