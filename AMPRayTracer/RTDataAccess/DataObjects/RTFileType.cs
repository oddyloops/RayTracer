using RTMeld.DataTransport;

namespace RTDataAccess.DataObjects
{
    public abstract class RTFileType : IRTFileType
    {
        private string id;

        private string ext;

        private string fullName;

     
        public virtual string Id { get => id; set => id = value; }
        public virtual string Ext { get => ext; set => ext = value; }
        public virtual string FullName { get => fullName; set => fullName = value; }

        public override bool Equals(object obj)
        {
            if(obj != null && obj is RTFileType)
            {
                var temp = (RTFileType)obj;
                return Id.Equals(temp.Id);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return id.GetHashCode();
        }
    }
}
