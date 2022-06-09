////////////////////////////////////////////////////////////////////
// Copyright (C) 2021 Thales Group. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using SafeNet.Sentinel;


namespace AdminApiSample
{
    class AdminApiSample
    {

        private static AdminApi adminApi;
        private static int vendorId = 37515;


        public static void Main()
        {
            string backupdata = "";

            Console.WriteLine("A simple demo program for the Sentinel LDK administration functions");
            Console.WriteLine("Copyright (C) Thales Group. All rights reserved.\n");

            adminApi = new AdminApi("localhost", 1947, "");

            //this initializes the adminApi
            sampleAdminConnect();

            //different samples
            commonSamples(ref backupdata);
            scopeSamples(vendorId);
            licenseMangerSample();
            keyDescriptionSample();
            restoreData(backupdata);
        }

        #region "samples"


        private static void commonSamples(ref string backupdata)
        {
            //Retrieve current context
            sampleAdminGet("", "<context></context>");

            //Retrieve all configuration settings (for backup)
            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"*\"/>" +
                " </config>" +
                "</admin>",
                ref backupdata
                );

            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"friendlyname\" />" +
                " </config>" +
                "</admin>"
                );

            sampleAdminSet(
                "<config>" +
                " <enabledetach>1</enabledetach>" +
                "</config>"
                );

            //Set configuration defaults and write to ini file
            sampleAdminSet(
                "<config>" +
                " <set_defaults />" +
                " <writeconfig />" +
                "</config>"
                );

            //Change some settings and write to ini file"
            //(For a list of available elements, see reply for \"*\" element below)
            sampleAdminSet(
                "<config>" +
                " <serveraddrs_clear/>" +
                " <serveraddr>10.24.2.16   </serveraddr>" +
                " <serveraddr>    10.24.2.16</serveraddr>" +
                " <serveraddr>p4p</serveraddr>" +
                " <serveraddr>10.24.2.255</serveraddr>" +
                " <user_restrictions_clear/>" +
                " <user_restriction>deny=baerbel@all</user_restriction>" +
                " <access_restrictions_clear />" +
                " <access_restriction>deny=10.23.*</access_restriction>" +
                " <enabledetach>1</enabledetach>" +
                " <writeconfig />" +
                "</config>"
                );

            //Retrieve all configuration settings
            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"*\" />" +
                " </config>" +
                "</admin>"
                );

            //Add some access restrictions
            sampleAdminSet(
                "<config>" +
                " <access_restriction>allow=123</access_restriction>" +
                " <access_restriction>allow=abcd</access_restriction>" +
                " <access_restriction>allow=hello_world</access_restriction>" +
                " <writeconfig />" +
                "</config>"
                );

            //Retrieve all access restrictions
            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"access_restriction\" />" +
                " </config>" +
                "</admin>"
                );

            //Add some more access restrictions
            sampleAdminSet(
                "<config>" +
                " <access_restriction>allow=more_123</access_restriction>" +
                " <access_restriction>allow=more_abcd</access_restriction>" +
                " <access_restriction>allow=more_hello_world</access_restriction>" +
                " <writeconfig />" +
                "</config>"
                );

            //Retrieve all access restrictions
            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"access_restriction\" />" +
                " </config>" +
                "</admin>"
                );

            //Delete existing access restrictions and add some new ones
            sampleAdminSet(
                "<config>" +
                " <access_restrictions_clear/>" +
                " <access_restriction>allow=new_123</access_restriction>" +
                " <access_restriction>allow=new_abcd</access_restriction>" +
                " <writeconfig />" +
                "</config>"
                );

            //Retrieve all access restrictions
            sampleAdminGet("",
                "<admin>" +
                " <config>" +
                "  <element name=\"access_restriction\" />" +
                " </config>" +
                "</admin>"
                );
        }

        //using haspscope to retrieve filtered data

        private static void scopeSamples(int vendorId)
        {
            //Retrieve some key data for specified vendor (scope with attribute notation)
            //(for a list of available elements, see reply for \"*\" element below)
            sampleAdminGet(
                "<haspscope>" +
                " <vendor id=\"" + vendorId + "\" />" +
                "</haspscope>", "<admin>" +
                " <hasp>\\n" +
                "  <element name=\"vendorid\" />" +
                "  <element name=\"haspid\" />" +
                "  <element name=\"typename\" />" +
                "  <element name=\"local\" />" +
                "  <element name=\"localname\" />" +
                " </hasp> " +
                "</admin>"
                );

            //Retrieve key data for specified vendor (scope with element notation)
            sampleAdminGet(
                "<haspscope>\\n" +
                " <vendor><id>" + vendorId + "</id></vendor>" +
                "</haspscope>", "<admin>" +
                " <hasp>" +
                "  <element name=\"vendorid\" />" +
                "  <element name=\"haspid\" />" +
                "  <element name=\"typename\" />" +
                "  <element name=\"local\" />" +
                "  <element name=\"localname\" />" +
                " </hasp>" +
                "</admin>"
                );

            //Retrieve all product data for specified vendor id"
            sampleAdminGet(
                "<haspscope>\\n" +
                " <vendor><id>" + vendorId + "</id></vendor>" +
                "</haspscope>", "<admin>" +
                " <product>" +
                "  <element name=\"*\" />" +
                " </product>" +
                "</admin>"
                );

            //Retrieve selected session data for all keys of a specified vendor
            sampleAdminGet(
                "<haspscope>\\n" +
                " <vendor><id>" + vendorId + "</id></vendor>" +
                "</haspscope>", "<admin>" +
                " <session>" +
                "  <element name=\"user\" />" +
                "  <element name=\"machine\" />" +
                "  <element name=\"logintime\" />" +
                " </session>" +
                "</admin>"
                );


        }

        //Sample where the scope uses the key-id
        private static void keyIdSamples(Int32 keyId)
        {
            //Retrieve all key data for specified key id"
            sampleAdminGet(
                "<haspscope>" +
                " <hasp><id>" + keyId + "</id></hasp>" +
                "</haspscope>", "<admin>" +
                " <hasp>" +
                "  <element name=\"*\" />" +
                " </hasp>" +
                "</admin>"
                );

            //Retrieve all feature data for specified key id"
            sampleAdminGet(
                "<haspscope>" +
                " <hasp><id>" + keyId + "</id></hasp>" +
                "</haspscope>", "<admin>" +
                " <feature>" +
                "  <element name=\"*\" />" +
                " </feature>" +
                "</admin>"
                );

            //Retrieve list of current sessions for a specified key
            sampleAdminGet(
                "<haspscope>" +
                " <hasp><id>" + keyId + "</id></hasp>" +
                "</haspscope>", "<admin>" +
                " <session>" +
                "  <element name=\"*\" />" +
                " </session>" +
                "</admin>"
                );

            //Retrieve all product data for specified key id"
            sampleAdminGet(
                "<haspscope>" +
                " <hasp><id>" + keyId + "</id></hasp>" +
                "</haspscope>", "<admin>" +
                " <product>" +
                "  <element name=\"productid\" />" +
                "  <element name=\"productname\" />" +
                "  <element name=\"detachable\" />" +
                "  <element name=\"maxseats\" />" +
                "  <element name=\"seatsfree\" />" +
                " </product>" +
                "</admin>"
                );
        }

        //Retrieve License Manager and license related data
        private static void licenseMangerSample()
	    {
		    //Retrieve license manager data in XML format (default)
		    sampleAdminGet("",
                "<admin>" +
                " <license_manager>" +
                "  <element name=\"*\" />" +
                " </license_manager>" +
                "</admin>"
                );

		    //Retrieve license manager data in JSON format
		    sampleAdminGet("",
                "<admin>" +
                " <license_manager format=\"json\">" +
                "  <element name=\"*\" />" +
                " </license_manager>" +
                "</admin>"
                );

		    //Retrieve list of detached licenses
		    sampleAdminGet("",
                "<admin>" +
                " <detached>" +
                "  <element name=\"*\" />" +
                " </detached>" +
                "</admin>"
                );
	    }

        //Setting a key description (disabled because it was not backed up)
        private static void keyDescriptionSample()
        {
            //Add a key description (legacy format)
            sampleAdminSet(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" +
                "<keydescription>" +
                " <hasp>" +
                "  <id>123456</id>" +
                "  <name>One two three four five six</name>" +
                " </hasp>" +
                "</keydescription>"
                );
        }

        //Examples for uploading of files (disabled because reverting not possible)
        private static void fileUploadSample()
        {
            //Upload a detach location data file
            sampleAdminSet("file://test_location.xml");

            //Upload a key names metadata file");
            sampleAdminSet("file://test_key.xml");

            //Upload a product names metadata file
            sampleAdminSet("file://test_product.xml");

            //Upload a vendor names metadata file
            sampleAdminSet("file://test_vendor.xml");

            //Applying V2C
            sampleAdminSet("file://test_update.v2c");
        }

        //Example how to delete a some sessions
        private static void deleteSessionSample()
        {
            sampleAdminSet(
                "<admin>" +
                " <deletesession>" +
                "  <sessionid>1</sessionid>" +
                "  <session id=\"2\" />" +
                "  <session id=\"3\" />" +
                "  <sessionid>4</sessionid>" +
                " </deletesession>" +
                "</admin>"
                );
        }

        //Retrieve list of certificates for specified key (XML)
        private static void certificateSample(Int32 keyId)
        {
            sampleAdminGet(
                "<haspscope>" +
                " <hasp><id>" + keyId + "</id></hasp>" +
                "</haspscope>", "<admin>" +
                " <certificates>" +
                "  <element name=\"*\" />" +
                " </certificates>" +
                "</admin>"
                );
        }

        #endregion

        #region "wrapper functions"

        private static void sampleAdminGet(string scope, string format)
        {
            string tempstring = "";

            sampleAdminGet(scope, format, ref tempstring);
        }

        private static void sampleAdminGet(string scope, string format, ref string target)
        {
            AdminStatus rc = default(AdminStatus);
            string info = "";

            Console.WriteLine("sntl_admin_get()");

            rc = adminApi.adminGet(scope, format, ref info);

            if (target != null)
            {
                target = info;
            }
            printState(rc, info);
        }

        private static void sampleAdminSet(string config)
        {
            AdminStatus rc = default(AdminStatus);
            string returnStatus = "";

            Console.WriteLine("sntl_admin_set()");
            rc = adminApi.adminSet(config, ref returnStatus);
            printState(rc, returnStatus);

        }

        private static void sampleAdminConnect()
        {
            AdminStatus rc = default(AdminStatus);

            rc = adminApi.connect();
            printState(rc);

        }

        #endregion

        #region "help functions"

        private static void restoreData(string backupdata)
        {
            string config = "";
            string returnStatus = "";

            generateConfigFromOutput(backupdata, ref config);

            adminApi.adminSet(
                "<config>" +
                " <serveraddrs_clear/>" +
                " <user_restrictions_clear/>" +
                " <access_restrictions_clear/>" +
                " <writeconfig/>" +
                "</config>",
                ref returnStatus
                );

            adminApi.adminSet(config, ref returnStatus);

        }

        private static void printState(AdminStatus status)
        {
            printState(status, null);
        }

        private static void printState(AdminStatus status, string info)
        {
            if (!(string.IsNullOrEmpty(info)))
            {
                Console.WriteLine(info);
            }

            Console.WriteLine("Result: " + getErrorText(status) + " Statuscode: " + (int)status + "\n");
        }

        private static void generateConfigFromOutput(string output, ref string config)
        {
            if (output != null)
            {
                Regex regex = new Regex("<admin_status>");
                string[] result = regex.Split(output);

                config = result[0].Replace("</config>", "<writeconfig /></config>");
            }
        }

        private static string getErrorText(AdminStatus status)
        {
            switch (status)
            {
                case AdminStatus.StatusOk: return "StatusOk";
                case AdminStatus.InsufMem: return "InsufMem";
                case AdminStatus.InvalidContext: return "InvalidContext";
                case AdminStatus.LmNotFound: return "LmNotFound";
                case AdminStatus.LmTooOld: return "LmTooOld";
                case AdminStatus.BadParameters: return "BadParameters";
                case AdminStatus.LocalNetWorkError: return "LocalNetWorkError";
                case AdminStatus.CannotReadFile: return "CannotReadFile";
                case AdminStatus.ScopeError: return "ScopeError";
                case AdminStatus.PasswordRequired: return "PasswordRequired";
                case AdminStatus.CannotSetPassword: return "CannotSetPassword";
                case AdminStatus.UpdateError: return "UpdateError";
                case AdminStatus.LocalOnly: return "LocalOnly";
                case AdminStatus.BadValue: return "BadValue";
                case AdminStatus.ReadOnly: return "ReadOnly";
                case AdminStatus.ElementUndefined: return "ElementUndefined";
                case AdminStatus.InvalidPointer: return "InvalidPointer";
                case AdminStatus.NoIntegratedLm: return "NoIntegratedLm";
                case AdminStatus.ResultTooBig: return "ResultTooBig";
                case AdminStatus.ScopeResultsEmpty: return "ScopeResultsEmpty";
                case AdminStatus.InvalidVendorCode: return "InvalidVendorCode";
                case AdminStatus.UnknownVendorCode: return "UnknownVendorCode";
                case AdminStatus.EntropySourceFailed:          return "EntropySourceFailed";
                case AdminStatus.InvCert:                      return "InvCert";
                case AdminStatus.CertVerifyFailed:             return "CertVerifyFailed";
                case AdminStatus.ConnectMissing:               return "ConnectMissing";
                case AdminStatus.DotNetDllBroken:              return "DotNetDllBroken";
                case AdminStatus.DLLLoadedInAnotherClassloader: return "DLLLoadedInAnotherClassloader";
                case AdminStatus.IdentityCodeRequired:         return "IdentityCodeRequired";
                case AdminStatus.IssuedToRequired:             return "IssuedToRequired";
                case AdminStatus.MaximumNumberOfAutoRegisteredMachinesRequired:  return "MaximumNumberOfAutoRegisteredMachinesRequired";
                case AdminStatus.IdentityCodeDoesNotExist:     return "IdentityCodeDoesNotExist";
                case AdminStatus.IdentityCodeCreationFailed:   return "IdentityCodeCreationFailed";
                case AdminStatus.IdentityNothingToUnregister:   return "IdentityNothingToUnregister";
            }
            return "";
        }

        #endregion

    }
}
