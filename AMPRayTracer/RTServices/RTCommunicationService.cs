using RTMeld.Services;
using System;
using System.Collections.Generic;
using System.Text;
using System.Composition;
using RTMeld.Enums;
using System.Threading.Tasks;
using System.Net.Mail;
using RTMeld.DataAccess;
using RTMeld.Config;
using System.Net;
using System.IO;
using System.Linq;

namespace RTServices
{
    [Export(typeof(ICommunicationService))]
    public class RTCommunicationService : ICommunicationService
    {
        [Import("JsonConfiguration")]
        public IConnectionContext ConfigContext { get; set; }

        [Import]
        public ISecurityService SecurityService { get; set; }

        [Import]
        public IKeyStoreService KeyStoreService { get; set; }

        public async Task<StatusCode> SendEmailAsync(string sender, string recipient, string subject, string body)
        {
            return await SendEmailAsync(sender, new List<string>() { recipient }, null, null, subject, body);
        }

        public Task<StatusCode> SendEmailAsync(string sender, IList<string> recipients, IList<string> ccs, IList<string> bccs, string subject, string body)
        {
            string host = ConfigContext.GetAppSetting(Config.MAIL_HOST);
            int port = int.Parse(ConfigContext.GetAppSetting(Config.MAIL_PORT));
            SmtpClient smtp = new SmtpClient(host,port);
            byte[] encryptedCredentials = File.ReadAllBytes(ConfigContext.GetAppSetting(Config.MAIL_CREDENTIAL_FILE));
            byte[] symmKey = KeyStoreService.GetKey(ConfigContext.GetAppSetting(Config.SYMMETRIC_KEY_INDEX));
            Tuple<string,string> decryptedCredentials = SecurityService.DecryptCredentials(encryptedCredentials, symmKey);
            string username = decryptedCredentials.Item1;
            string password = decryptedCredentials.Item2;
            smtp.Credentials = new NetworkCredential(username,password);
            IList<MailAddress> receivers = (from r in recipients select new MailAddress(r)).ToList();

            IList<MailAddress> copies = (ccs == null ? new List<MailAddress>() :
                (from c in ccs select new MailAddress(c)).ToList());
            IList<MailAddress> blindCopies = (bccs == null ? new List<MailAddress>() :
                (from b in bccs select new MailAddress(b)).ToList());

            MailAddress senderAddress = new MailAddress(username, sender);

            MailMessage message = new MailMessage();
            message.Subject = subject;
            message.Sender = senderAddress;

            foreach(var address in receivers)
            {
                message.To.Add(address);
            }

            foreach(var address in copies)
            {
                message.CC.Add(address);
            }

            foreach(var address in blindCopies)
            {
                message.Bcc.Add(address);
            }

            message.IsBodyHtml = true;
            message.Body = body;

            smtp.Send(message);

            return Task.FromResult(StatusCode.Successful);

        }
    }
}
