
using RTMeld.Attributes;
using System;

namespace RTMeld.DataTransport
{
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public interface IRTFileType
    {
   
        [Key]
        [Map("id,ID")]
        string Id { get; set; }

        [Map("extension,ext")]
        string Ext { get; set; }

        [Map("full_name,Fullname,fullname")]
        string FullName { get; set; }
    }
}
