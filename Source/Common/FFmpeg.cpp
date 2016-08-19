/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a GPLv3+/MPLv2+ license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

#include <ZenLib/Ztring.h>
#include <ZenLib/File.h>
#include <ZenLib/FileName.h>
#include "FFmpeg.h"

//---------------------------------------------------------------------------
namespace MediaConch {

    //***************************************************************************
    // Constructor/Destructor
    //***************************************************************************

    //---------------------------------------------------------------------------
    FFmpeg::FFmpeg()
    {
        type = MediaConchLib::PLUGIN_PRE_HOOK;
    }

    //---------------------------------------------------------------------------
    FFmpeg::~FFmpeg()
    {
    }

    //---------------------------------------------------------------------------
    int FFmpeg::load_plugin(const std::map<std::string, Container::Value>& obj, std::string& error)
    {
        if (obj.find("name") == obj.end() || obj.at("name").type != Container::Value::CONTAINER_TYPE_STRING)
        {
            error += "Field 'name' is not present\n";
            return -1;
        }
        name = obj.at("name").s;

        if (obj.find("bin") == obj.end() || obj.at("bin").type != Container::Value::CONTAINER_TYPE_STRING)
        {
            error += "Field 'bin' is not present\n";
            return -1;
        }
        bin = obj.at("bin").s;

        if (obj.find("outputDir") == obj.end() || obj.at("outputDir").type != Container::Value::CONTAINER_TYPE_STRING)
        {
            error += "Field 'outputDir' is not present\n";
            return -1;
        }
        outputDir = obj.at("outputDir").s;
        if (outputDir[outputDir.length() - 1] != '/')
            outputDir += "/";

        if (obj.find("outputExt") == obj.end() || obj.at("outputExt").type != Container::Value::CONTAINER_TYPE_STRING)
        {
            error += "Field 'outputExt' is not present\n";
            return -1;
        }
        outputExt = obj.at("outputExt").s;

        size_t pos = 0;
        while (1)
        {
            pos = outputDir.find("\\", pos);
            if (pos == std::string::npos)
                break;

            outputDir[pos] = '/';
        }

        if (obj.find("inputParams") != obj.end() && obj.at("inputParams").type == Container::Value::CONTAINER_TYPE_ARRAY)
        {
            for (size_t i = 0; i < obj.at("inputParams").array.size(); ++i)
            {
                const Container::Value& val = obj.at("inputParams").array[i];
                if (val.type == Container::Value::CONTAINER_TYPE_STRING)
                    inputParams.push_back(val.s);
            }
        }

        if (obj.find("outputParams") != obj.end() && obj.at("outputParams").type == Container::Value::CONTAINER_TYPE_ARRAY)
        {
            for (size_t i = 0; i < obj.at("outputParams").array.size(); ++i)
            {
                const Container::Value& val = obj.at("outputParams").array[i];
                if (val.type == Container::Value::CONTAINER_TYPE_STRING)
                    outputParams.push_back(val.s);
            }
        }

        if (obj.find("params") != obj.end() && obj.at("params").type == Container::Value::CONTAINER_TYPE_ARRAY)
        {
            for (size_t i = 0; i < obj.at("params").array.size(); ++i)
            {
                const Container::Value& val = obj.at("params").array[i];
                if (val.type == Container::Value::CONTAINER_TYPE_STRING)
                    params.push_back(val.s);
            }
        }

        return 0;
    }

    //---------------------------------------------------------------------------
    int FFmpeg::run(std::string& error)
    {
        outputFile.clear();

        if (!inputFile.length())
            return 0;

        if (!outputDir.length())
            outputDir = ".";

        create_output_file_name();

        std::vector<std::string> exec_params;

        exec_params.push_back(bin);
        for (size_t i = 0; i < inputParams.size(); ++i)
            exec_params.push_back(inputParams[i]);

        exec_params.push_back("-i");
        exec_params.push_back(inputFile);

        for (size_t i = 0; i < outputParams.size(); ++i)
            exec_params.push_back(outputParams[i]);

        exec_params.push_back(outputFile);

        for (size_t i = 0; i < params.size(); ++i)
            exec_params.push_back(params[i]);

        return exec_bin(exec_params, error);
    }

    //---------------------------------------------------------------------------
    void FFmpeg::create_output_file_name()
    {
        ZenLib::Ztring z_in = ZenLib::Ztring().From_UTF8(inputFile);
        ZenLib::FileName file(z_in);
        std::string out = file.Name_Get().To_UTF8();

        for (size_t i = 0; ; ++i)
        {
            std::stringstream path;

            path << outputDir << out;
            if (i)
                path << i;
            path << "." << outputExt;

            outputFile = path.str();
            if (!ZenLib::File::Exists(ZenLib::Ztring().From_UTF8(outputFile)))
                break;
        }
    }

}
