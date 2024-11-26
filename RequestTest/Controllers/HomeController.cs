using System.Net.Sockets;
using System.Text;
using Microsoft.AspNetCore.Mvc;
using RequestTest.Models;


namespace RequestTest.Controllers
{
    public class HomeController : Controller
    {
        private string ConnectToCppServer()
        {
            try
            {
                string serverIP = "127.0.0.1";
                int port = 8080;

                using (TcpClient client = new TcpClient(serverIP, port))
                {
                    Console.WriteLine("Connected to the server!");

                    // Send 
                    string request = "Send CSV file";
                    byte[] requestData = Encoding.UTF8.GetBytes(request);
                    client.GetStream().Write(requestData, 0, requestData.Length);

                    // Receive 
                    byte[] responseData = new byte[10240];
                    int bytesRead = client.GetStream().Read(responseData, 0, responseData.Length);
                    string response = Encoding.UTF8.GetString(responseData, 0, bytesRead);

                    return response; 
                }
            }
            catch (Exception ex)
            {
                return "Error: " + ex.Message;
            }
        }

        public ActionResult Index()
        {
            return View(new RequestResponseModel());
        }

        [HttpPost]
        public ActionResult Index(RequestResponseModel model)
        {           
            string csvData = ConnectToCppServer();

            model.ResponseMessage = csvData;
            return View(model);
        }
    }
}
