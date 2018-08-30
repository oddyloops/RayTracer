using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Composition;
using System.Text;
using RTMeld.DataAccess;
using RTMeld.DataTransport;
using RTMeld.Enums;
using System.Threading.Tasks;

namespace RTServices
{
    [Export(typeof(ICrudService))]
    public class RTCrudService : ICrudService
    {
        public async Task<StatusCode> CreateAsync(IDataContext context, IData data)
        {
            await context.InsertAsync(data);
            return StatusCode.Successful;
        }

        public async Task<StatusCode> DeleteAsync(IDataContext context, object id)
        {
            await context.DeleteAsync<IData>(id);
            return StatusCode.Successful;
        }

        public async Task<IData> ReadAsync(IDataContext context, object id)
        {
            return await context.SelectOneAsync<IData>(id);
        }

        public async Task<StatusCode> UpdateAsync(IDataContext context, object id, IData newData, bool excludeNulls = false)
        {
            int result = await context.UpdateAsync<IData>(id, newData, true);
            return (result == 0 ? StatusCode.Successful : StatusCode.NotFound);
        }
    }
}
