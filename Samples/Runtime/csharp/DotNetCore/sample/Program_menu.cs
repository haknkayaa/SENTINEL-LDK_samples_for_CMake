using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using Aladdin.HASP;


namespace netcore_test_sample
{
    class Program
    {
        static void Main(string[] args)
        {
            string license = null;
            string scope = null;
            string fingerprint_format = "<haspformat format=\"host_fingerprint\"/>";
            string info = null;
            string vendor_code = null;
            string temp = null;
            string ack_data = null;
            HaspStatus status;
            int f_id = 0;
            int num, n;
            char ch;
            string DEMOMA = "AzIceaqfA1hX5wS+M8cGnYh5ceevUnOZIzJBbXFD6dgf3tBkb9cvUF/Tkd/iKu2fsg9wAysYKw7RMAsV" +
                                "vIp4KcXle/v1RaXrLVnNBJ2H2DmrbUMOZbQUFXe698qmJsqNpLXRA367xpZ54i8kC5DTXwDhfxWTOZrB" +
                                "rh5sRKHcoVLumztIQjgWh37AzmSd1bLOfUGI0xjAL9zJWO3fRaeB0NS2KlmoKaVT5Y04zZEc06waU2r6" +
                                "AU2Dc4uipJqJmObqKM+tfNKAS0rZr5IudRiC7pUwnmtaHRe5fgSI8M7yvypvm+13Wm4Gwd4VnYiZvSxf" +
                                "8ImN3ZOG9wEzfyMIlH2+rKPUVHI+igsqla0Wd9m7ZUR9vFotj1uYV0OzG7hX0+huN2E/IdgLDjbiapj1" +
                                "e2fKHrMmGFaIvI6xzzJIQJF9GiRZ7+0jNFLKSyzX/K3JAyFrIPObfwM+y+zAgE1sWcZ1YnuBhICyRHBh" +
                                "aJDKIZL8MywrEfB2yF+R3k9wFG1oN48gSLyfrfEKuB/qgNp+BeTruWUk0AwRE9XVMUuRbjpxa4YA67SK" +
                                "unFEgFGgUfHBeHJTivvUl0u4Dki1UKAT973P+nXy2O0u239If/kRpNUVhMg8kpk7s8i6Arp7l/705/bL" +
                                "Cx4kN5hHHSXIqkiG9tHdeNV8VYo5+72hgaCx3/uVoVLmtvxbOIvo120uTJbuLVTvT8KtsOlb3DxwUrwL" +
                                "zaEMoAQAFk6Q9bNipHxfkRQER4kR7IYTMzSoW5mxh3H9O8Ge5BqVeYMEW36q9wnOYfxOLNw6yQMf8f9s" +
                                "JN4KhZty02xm707S7VEfJJ1KNq7b5pP/3RjE0IKtB2gE6vAPRvRLzEohu0m7q1aUp8wAvSiqjZy7FLaT" +
                                "tLEApXYvLvz6PEJdj4TegCZugj7c8bIOEqLXmloZ6EgVnjQ7/ttys7VFITB3mazzFiyQuKf4J6+b/a/Y";

            vendor_code = DEMOMA;

            Console.WriteLine("\n\t              ---[ DotNet Core Runtime API Sample ]---\n");


            Console.WriteLine("\n\t                 DotNet Runtime API\n");
            Console.Write("\n\t Enter a feature id : ");
            temp = Console.ReadLine();

            while (!(int.TryParse(temp, out f_id)))
            {
                Console.Write("\n\t Please enter an integer.");
                Console.Write("\n\t Enter a feature id : ");
                temp = Console.ReadLine();
            }

            f_id = Convert.ToInt32(temp);
            HaspFeature feature = new HaspFeature(f_id);
            Hasp hasp_obj = new Hasp(feature);
                
            do
            {

                Console.WriteLine("\n\t <-----------------MENU----------------->\n");
                Console.WriteLine("\n\t 1. Login");
                Console.WriteLine("\n\t 2. Logout");
                Console.WriteLine("\n\t 3. Encrypt");
                Console.WriteLine("\n\t 4. Decrypt");
                Console.WriteLine("\n\t 5. Read");
                Console.WriteLine("\n\t 6. Write");
                Console.WriteLine("\n\t 7. GetRTC");
                Console.WriteLine("\n\t 8. Update");
                Console.WriteLine("\n\t 9. GetInfo");
                Console.WriteLine("\n\t10. GetSessionInfo");
                Console.WriteLine("\n\t11. Exit");
                Console.Write("\n\n\t Enter your choice: ");

                temp=Console.ReadLine();

                while (!(int.TryParse(temp, out num)))
                {
                    Console.Write("\n\t Please enter an integer.");
                    Console.Write("\n\n\t Enter your choice: ");
                    temp = Console.ReadLine();
                }

                num =(int)Convert.ToUInt32(temp);
    
                switch(num)
                {
                    /* LOGIN API */
                    case 1:		Console.WriteLine("\n\t		***LOGIN FUNCTION***");
                                status = hasp_obj.Login(vendor_code);
                                if (status != HaspStatus.StatusOk)
                                {
                                     Console.WriteLine("\n\t Login API failed with error status : {0}", status);
                                     Console.ReadKey();
                                     System.Environment.Exit(1);
     
                                }
                                Console.WriteLine("\n\t Successfully Logged into the feature {0}", f_id);
                                break;
    
                    /* LOGOUT API */
                    case 2:		Console.WriteLine("\n\t		***LOGOUT FUNCTION***");
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Logout, no feature Logged-in...");
                                    break;
                                }
                                status = hasp_obj.Logout();
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Logout API failed with error status: {0}", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
    
                                }
                                Console.WriteLine("\n\t Successfully Logged-out");
                                break;
    
                    /* ENCRYPT API */
                    case 3:		Console.WriteLine("\n\t		***ENCRYPT FUNCTION***");

                                temp =  "This is a sample test document for\n" +
                                        "Encyption / Decryption flow check";

                                char[] data=temp.ToCharArray();
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Encrypt data, no feature Logged-in...");
                                    break;
                                }
                                status = hasp_obj.Encrypt(ref temp);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Encrypt API failed with error status: {0}", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
    
                                }

                                System.IO.File.WriteAllText(@"Encrypted.txt",temp );
                                Console.WriteLine("\n\t File Successfully Encrypted....");
                                Console.WriteLine("\n\t Encrypted file is shown below: \n");
                                Console.WriteLine(temp);
                            
                                break;
    
                    /* DECRYPT API */
                    case 4:		Console.WriteLine("\n\t		***DECRYPT FUNCTION FUNCTION***");
                                temp = System.IO.File.ReadAllText(@"Encrypted.txt");	
                                data=temp.ToCharArray();
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Decrypt data, no feature Logged-in...");

                                    break;
                                }
                                status = hasp_obj.Decrypt(ref temp);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Decrypt API failed with error status: {0}", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
                                }

                                System.IO.File.WriteAllText(@"Decrypted.txt",temp);
                                Console.WriteLine("\n\t File Successfully Decrypted....");
                                Console.WriteLine("\n\t Decrypted file is shown below: \n");
                                Console.WriteLine(temp);

                                break;
    
                    /* READ API */
                    case 5:
                                Console.WriteLine("\n\t		***READ FUNCTION***");
                                Console.WriteLine("\n\n\t Specify fileid: ");
                                Console.WriteLine("\n\t 1. RO - Default read-only memory");
                                Console.WriteLine("\n\t 2. RW - Default read/write memory");
                                Console.Write("\n\t Enter your choice: ");
                                temp=Console.ReadLine();

                                while (!(int.TryParse(temp, out n)))
                                {
                                    Console.Write("\n\t Please enter an integer.");
                                    Console.Write("\n\t Enter your choice: ");
                                    temp = Console.ReadLine();
                                }

                                n=Convert.ToInt32(temp);
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Logout, no feature Logged-in...");
                                    break;
                                }
                                HaspFile file_read=new HaspFile(HaspFileId.None,hasp_obj);
                                string read_data=null;
                                switch(n)
                                {
                                    case 1:		file_read=hasp_obj.GetFile(HaspFileId.ReadOnly);
                                                break;
                                    case 2:		file_read=hasp_obj.GetFile(HaspFileId.ReadWrite);
                                                break;
                                    default:	Console.WriteLine("\n\t Invalid Choice....");
                                                Console.ReadKey();
                                                System.Environment.Exit(0);	
                                                break;
                                }
                                status=file_read.Read(ref read_data);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Read API failed with error status: {0}", status);
                                    Console.ReadKey();	
                                    System.Environment.Exit(1);
    
                                }
                                Console.WriteLine("\n\t Data Read operation successful, read data is shown below: ");
                                Console.WriteLine(read_data);
    
                                break;
                           
                    /* WRITE API */
                    case 6:
    
                                Console.WriteLine("\n\t		***WRITE FUNCTION***");
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Logout, no feature Logged-in...");
                                    break;
                                }         
                                HaspFile file_write=hasp_obj.GetFile(HaspFileId.ReadWrite);
                                string write_data=null;
                                Console.Write("\n\t Enter data to be written to RW memory: ");
                                write_data= Console.ReadLine();
                                
                                status=file_write.Write(write_data);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Write API failed with error status: {0}", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
    
                                }
    
                                Console.WriteLine("\n\t Data Successfully written to the RW memory of Key...");
                                
                                break;
    

                    /* GETRTC API */
                    case 7:		
                                Console.WriteLine("\n\t		***GETRTC FUNCTION***");
                                DateTime mydate=new DateTime();
                                status=hasp_obj.GetRtc(ref mydate);
                                if (!hasp_obj.IsLoggedIn())
                                {
                                    Console.WriteLine("\n\t Cannot Logout, no feature Logged-in...");
                                    break;
                                }
                                if(status!=HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t GETRTC API failed with error status: {0}", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
                                }
                                Console.WriteLine("\n\t RTC Time: {0}", mydate);
                                break;
    
                    /* UPDATE API */
                    case 8:		
                                license = System.IO.File.ReadAllText(@"license.v2c");   // license.v2c needs to be present in the same directory to update license
                                status =Hasp.Update(license, ref ack_data);
                                if(status!=HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t Update License API failed with error status: {0}", status);
                                }
                                Console.WriteLine("\n\t License information successfully updated...");
                                break;

                    /* GET_INFO API */
                    case 9:
                                Console.Write("\n\t		 -----------------------");
                                Console.WriteLine("\n\t		|***GET_INFO FUNCTION***|");

                                scope =
                                                "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" +
                                                "<haspscope>" +
                                                "    <license_manager hostname=\"localhost\" />" +
                                                "</haspscope>";


                                status = Hasp.GetInfo(scope, fingerprint_format, vendor_code, ref info);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\tGetInfo API (fingerprint information) failed with error status: {0}...", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
                                }
                                Console.WriteLine("\n\t Fingerprint Info Fetched successfully...");
                                Console.WriteLine("\n Fingerprint Info: \n {0} \n", info);

                                break;

                    /* GET_SESSION_INFO API */
                    case 10:
                                Console.Write("\n\t		 -------------------------------");
                                Console.WriteLine("\n\t		|***GET_SESSION_INFO FUNCTION***|");

                                info = null;

                                status = hasp_obj.GetSessionInfo(Hasp.SessionInfo, ref info);
                                if (status != HaspStatus.StatusOk)
                                {
                                    Console.WriteLine("\n\t GET_SESSION_INFO API failed with error status: {0}...", status);
                                    Console.ReadKey();
                                    System.Environment.Exit(1);
                                }
                                Console.WriteLine("\n\t Session Info fetched successfully...");
                                Console.WriteLine("\n Session Info: \n {0} \n", info);
                                break;


                    /* EXIT */
                    case 11:
                                System.Environment.Exit(0);
                                break;

                    default:	Console.WriteLine("\n\t Invalid Input");
                                Console.ReadKey();
                                System.Environment.Exit(0);
                                break;
                
    
                }
                Console.Write("\n\t Do you want to continue(Y for yes): ");
                ch=Console.ReadKey().KeyChar;

            }while(ch=='Y'||ch=='y');

            System.Console.ReadKey();
        }
    }
}
