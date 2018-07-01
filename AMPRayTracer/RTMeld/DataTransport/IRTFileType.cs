
using RTMeld.Attributes;

namespace RTMeld.DataTransport
{
    [Meta(AzureCosmosDocCollection ="rt_file_type")]
    public interface IRTFileType
    {
        [Key]
        [Map("id")]
        int Id { get; set; }

        [Map("extension,ext")]
        string Ext { get; set; }

        [Map("full_name,Fullname,fullname")]
        string FullName { get; set; }
    }
}
