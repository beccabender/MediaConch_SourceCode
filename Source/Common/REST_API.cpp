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

//---------------------------------------------------------------------------
#include <sstream>
#include "REST_API.h"
#include "Container.h"
#include "NoContainer.h"
#include "Json.h"
#include "stdlib.h"

//---------------------------------------------------------------------------
namespace MediaConch {

//***************************************************************************
// RESTAPI
//***************************************************************************

const std::string RESTAPI::API_VERSION = "1.4";

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
RESTAPI::RESTAPI()
{
#ifdef HAVE_JANSSON
    model = new Json;
#else
    model = new NoContainer;
#endif
}

//---------------------------------------------------------------------------
RESTAPI::~RESTAPI()
{
    if (model)
    {
        delete model;
        model = NULL;
    }
}

//***************************************************************************
// Destructors
//***************************************************************************

//---------------------------------------------------------------------------
RESTAPI::Analyze_Res::~Analyze_Res()
{
    for (size_t i = 0; i < ok.size(); ++i)
        delete ok[i];
    ok.clear();

    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//---------------------------------------------------------------------------
RESTAPI::Status_Res::~Status_Res()
{
    for (size_t i = 0; i < ok.size(); ++i)
        delete ok[i];
    ok.clear();

    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//---------------------------------------------------------------------------
RESTAPI::Report_Res::~Report_Res()
{
    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//---------------------------------------------------------------------------
RESTAPI::Retry_Res::~Retry_Res()
{
    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//---------------------------------------------------------------------------
RESTAPI::Clear_Res::~Clear_Res()
{
    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//---------------------------------------------------------------------------
RESTAPI::List_Res::~List_Res()
{
    for (size_t i = 0; i < files.size(); ++i)
        delete files[i];
    files.clear();
}

//---------------------------------------------------------------------------
RESTAPI::Validate_Res::~Validate_Res()
{
    for (size_t i = 0; i < ok.size(); ++i)
        delete ok[i];
    ok.clear();
    for (size_t i = 0; i < nok.size(); ++i)
        delete nok[i];
    nok.clear();
}

//***************************************************************************
// Request: to_str()
//***************************************************************************

//---------------------------------------------------------------------------
std::string RESTAPI::Analyze_Arg::to_str() const
{
    std::stringstream out;

    out << "[file: '" << file << "', id: '" << id << "', force: '";
    if (!has_force_analyze || !force_analyze)
        out << "false";
    else
        out << "true";
    out << "]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Analyze_Req::to_str() const
{
    std::stringstream out;

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i)
            out << ", ";
        out << args[i].to_str();
    }
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Status_Req::to_str() const
{
    std::stringstream out;

    out << "[ids: [";
    for (size_t i = 0; i < ids.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ids[i];
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Report_Req::to_str() const
{
    std::stringstream out;

    out << "[ids: [";
    for (size_t i = 0; i < ids.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ids[i];
    }
    out << "], reports: [";
    for (size_t i = 0; i < reports.size(); ++i)
    {
        if (i)
            out << ", ";
        RESTAPI api;
        out << api.get_Report_string(reports[i]);
    }
    out << "], policies_names_size: [" << policies_names.size();
    out << "], policies_contents_size: [" << policies_contents.size();
    out << "], display_name: [" << display_name;
    out << "], display_content_length: [" << display_content.size();
    out << "]";
    if (has_verbosity)
        out << ", verbosity: " << verbosity;
    out << " ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Retry_Req::to_str() const
{
    std::stringstream out;

    out << "[ids: [";
    for (size_t i = 0; i < ids.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ids[i];
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Clear_Req::to_str() const
{
    std::stringstream out;

    out << "[ids: [";
    for (size_t i = 0; i < ids.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ids[i];
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Validate_Req::to_str() const
{
    std::stringstream out;

    out << "[ids: [";
    for (size_t i = 0; i < ids.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ids[i];
    }
    RESTAPI api;
    out << "], report: " << api.get_Report_string(report);
    out << ", policies_names_size: [" << policies_names.size();
    out << "], policies_contents_size: [" << policies_contents.size() << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::File_From_Id_Req::to_str() const
{
    std::stringstream out;

    out << "[id: '" << id << "']";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Default_Values_For_Type_Req::to_str() const
{
    std::stringstream out;

    out << "{type: '" << type << "'}";
    return out.str();
}

//***************************************************************************
// Result: to_str()
//***************************************************************************

//---------------------------------------------------------------------------
std::string RESTAPI::Analyze_Ok::to_str() const
{
    std::stringstream out;

    out << "{inId: " << inId;
    out << ", outId: " << outId;
    out << ", create: " << std::boolalpha << create << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Analyze_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Analyze_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: [";
    for (size_t i = 0; i < ok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ok[i]->to_str();
    }
    out << "], nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Status_Ok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    out << ", finished: " << std::boolalpha << finished;
    if (has_percent)
        out << ", done: " << done;
    if (finished && has_tool)
    {
        RESTAPI api;
        out << ", tool: " << api.get_Report_string(tool);
    }
    out << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Status_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Status_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: [";
    for (size_t i = 0; i < ok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ok[i]->to_str();
    }
    out << "], nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Report_Ok::to_str() const
{
    std::stringstream out;

    out << "{report_length: " << report.length();
    if (has_valid)
        out << ", valid: " << std::boolalpha << valid;
    out << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Report_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Report_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: " << ok.to_str() << ", nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Retry_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Retry_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: [";
    for (size_t i = 0; i < ok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ok[i];
    }
    out << "], nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Clear_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Clear_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: [";
    for (size_t i = 0; i < ok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ok[i];
    }
    out << "], nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::List_Res::to_str() const
{
    std::stringstream out;

    out << "[files: [";
    for (size_t i = 0; i < files.size(); ++i)
    {
        if (i)
            out << ", ";
        out << "{file:" << files[i]->file;
        out << ", id:" << files[i]->id << "}";
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Validate_Ok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id << ", valid: " << std::boolalpha << valid << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Validate_Nok::to_str() const
{
    std::stringstream out;

    out << "{id: " << id;
    RESTAPI api;
    out << ", reason: " << api.get_Reason_string(error) << "}";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Validate_Res::to_str() const
{
    std::stringstream out;

    out << "[ok: [";
    for (size_t i = 0; i < ok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << ok[i]->to_str();
    }
    out <<  "], nok: [";
    for (size_t i = 0; i < nok.size(); ++i)
    {
        if (i)
            out << ", ";
        out << nok[i]->to_str();
    }
    out << "] ]";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::File_From_Id_Res::to_str() const
{
    std::stringstream out;

    out << "[file: '" << file << "']";
    return out.str();
}

//---------------------------------------------------------------------------
std::string RESTAPI::Default_Values_For_Type_Res::to_str() const
{
    std::stringstream out;

    out << "[values: '";
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i)
            out << ",";
        out << values[i];
    }
    out  << "']";
    return out.str();
}

//***************************************************************************
// Serialize: Request
//***************************************************************************

//---------------------------------------------------------------------------
int RESTAPI::serialize_analyze_req(Analyze_Req& req, std::string& data)
{
    Container::Value v, child;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["args"] = serialize_analyze_args(req.args);

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["ANALYZE"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_status_req(Status_Req& req, std::string& data)
{
    //URI
    std::stringstream ss;

    if (req.ids.size() > 0)
        ss << "?";
    for (size_t i = 0; i < req.ids.size(); ++i)
    {
        if (i)
            ss << "&";
        ss << "id=" << req.ids[i];
    }
    data = ss.str();

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_report_req(Report_Req& req, std::string& data)
{
    Container::Value v, child;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ids"] = serialize_ids(req.ids);
    child.obj["reports"] = serialize_report_reports(req.reports);

    if (req.policies_names.size())
        child.obj["policies_names"] = serialize_report_arr_str(req.policies_names);
    if (req.policies_contents.size())
        child.obj["policies_contents"] = serialize_report_arr_str(req.policies_contents);

    if (req.display_name.length())
        child.obj["display_name"] = serialize_report_string(req.display_name);
    if (req.display_content.length())
        child.obj["display_content"] = serialize_report_string(req.display_content);

    if (req.has_verbosity)
        child.obj["verbosity"] = serialize_report_int(req.verbosity);

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["REPORT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_retry_req(Retry_Req& req, std::string& data)
{
    Container::Value v, child;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ids"] = serialize_ids(req.ids);

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["RETRY"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_clear_req(Clear_Req& req, std::string& data)
{
    //URI
    std::stringstream ss;

    if (req.ids.size() > 0)
        ss << "?";
    for (size_t i = 0; i < req.ids.size(); ++i)
    {
        if (i)
            ss << "&";
        ss << "id=" << req.ids[i];
    }
    data = ss.str();

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_list_req(List_Req&, std::string& data)
{
    data = std::string();
    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_validate_req(Validate_Req& req, std::string& data)
{
    Container::Value v, child, report;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ids"] = serialize_ids(req.ids);

    report.type = Container::Value::CONTAINER_TYPE_STRING;
    report.s = get_Report_string(req.report);
    child.obj["report"] = report;

    if (req.policies_names.size())
        child.obj["policies_names"] = serialize_report_arr_str(req.policies_names);
    if (req.policies_contents.size())
        child.obj["policies_contents"] = serialize_report_arr_str(req.policies_contents);

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["VALIDATE"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_file_from_id_req(File_From_Id_Req& req, std::string& data)
{
    Container::Value v, child, id;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;

    id.type = Container::Value::CONTAINER_TYPE_INTEGER;
    id.l = req.id;

    child.obj["id"] = id;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["FILE_FROM_ID"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_default_values_for_type_req(Default_Values_For_Type_Req& req, std::string& data)
{
    Container::Value v, child, type;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;

    type.type = Container::Value::CONTAINER_TYPE_STRING;
    type.s = req.type;

    child.obj["type"] = type;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["DEFAULT_VALUES_FOR_TYPE"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//***************************************************************************
// Serialize: Result
//***************************************************************************

//---------------------------------------------------------------------------
int RESTAPI::serialize_analyze_res(Analyze_Res& res, std::string& data)
{
    Container::Value v, child, nok;

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = serialize_analyze_oks(res.ok);
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["ANALYZE_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }
    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_status_res(Status_Res& res, std::string& data)
{
    Container::Value v, child, nok;

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = serialize_status_oks(res.ok);
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["STATUS_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_report_res(Report_Res& res, std::string& data)
{
    Container::Value v, child, nok;

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = serialize_report_ok(res.ok);
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["REPORT_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_retry_res(Retry_Res& res, std::string& data)
{
    Container::Value v, child, nok;

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = serialize_ids(res.ok);
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["RETRY_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_clear_res(Clear_Res& res, std::string& data)
{
    Container::Value v, child, nok;

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = serialize_ids(res.ok);
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["CLEAR_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }
    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_list_res(List_Res& res, std::string& data)
{
    Container::Value v, child, files;

    files.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.files.size(); ++i)
        files.array.push_back(serialize_list_file(res.files[i]->file, res.files[i]->id));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["files"] = files;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["LIST_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_validate_res(Validate_Res& res, std::string& data)
{
    Container::Value v, child, ok, nok;

    ok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.ok.size(); ++i)
        ok.array.push_back(serialize_validate_ok(res.ok[i]));

    nok.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.nok.size(); ++i)
        nok.array.push_back(serialize_generic_nok(res.nok[i]->id, res.nok[i]->error));

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["ok"] = ok;
    child.obj["nok"] = nok;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["VALIDATE_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_file_from_id_res(File_From_Id_Res& res, std::string& data)
{
    Container::Value v, child, file;

    file.type = Container::Value::CONTAINER_TYPE_STRING;
    file.s = res.file;

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["file"] = file;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["FILE_FROM_ID_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::serialize_default_values_for_type_res(Default_Values_For_Type_Res& res, std::string& data)
{
    Container::Value v, child, values;

    values.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < res.values.size(); ++i)
    {
        Container::Value value;
        value.type = Container::Value::CONTAINER_TYPE_STRING;
        value.s = res.values[i];
        values.array.push_back(value);
    }

    child.type = Container::Value::CONTAINER_TYPE_OBJECT;
    child.obj["values"] = values;

    v.type = Container::Value::CONTAINER_TYPE_OBJECT;
    v.obj["DEFAULT_VALUES_FOR_TYPE_RESULT"] = child;

    data = model->serialize(v);
    if (!data.length())
    {
        error = model->get_error();
        return -1;
    }

    return 0;
}

//***************************************************************************
// Serialize: Result
//***************************************************************************

//---------------------------------------------------------------------------
RESTAPI::Analyze_Req *RESTAPI::parse_analyze_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "ANALYZE");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;
    Analyze_Req *req = new Analyze_Req;

    Container::Value *args;
    args = model->get_value_by_key(*child, "args");

    if (!args || parse_analyze_arg(args, req->args) < 0)
    {
        delete req;
        return NULL;
    }
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Status_Req *RESTAPI::parse_status_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "STATUS");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Status_Req *req = new Status_Req;

    Container::Value *ids;
    ids = model->get_value_by_key(*child, "ids");

    if (!ids || ids->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete req;
        return NULL;
    }

    for (size_t i = 0; i < ids->array.size(); ++i)
    {
        Container::Value *id = &ids->array[i];

        if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        {
            delete req;
            return NULL;
        }
        req->ids.push_back(id->l);
    }
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Report_Req *RESTAPI::parse_report_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "REPORT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ids, *reports, *policies_names, *policies_contents, *display_name, *display_content, *verbosity;
    ids = model->get_value_by_key(*child, "ids");
    reports = model->get_value_by_key(*child, "reports");
    policies_names = model->get_value_by_key(*child, "policies_names");
    policies_contents = model->get_value_by_key(*child, "policies_contents");
    display_name = model->get_value_by_key(*child, "display_name");
    display_content = model->get_value_by_key(*child, "display_content");
    verbosity = model->get_value_by_key(*child, "verbosity");

    if (!ids || !reports || ids->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return NULL;

    Report_Req *req = new Report_Req;
    for (size_t i = 0; i < ids->array.size(); ++i)
    {
        Container::Value *id = &ids->array[i];

        if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        {
            delete req;
            return NULL;
        }
        req->ids.push_back(id->l);
    }
    if (parse_report_reports(reports, req->reports))
    {
        delete req;
        return NULL;
    }

    if (policies_names && policies_names->type == Container::Value::CONTAINER_TYPE_ARRAY)
    {
        for (size_t i = 0; i < policies_names->array.size(); ++i)
            if (policies_names->array[i].type == Container::Value::CONTAINER_TYPE_STRING)
                req->policies_names.push_back(policies_names->array[i].s);
    }
    if (policies_contents && policies_contents->type == Container::Value::CONTAINER_TYPE_ARRAY)
    {
        for (size_t i = 0; i < policies_contents->array.size(); ++i)
            if (policies_contents->array[i].type == Container::Value::CONTAINER_TYPE_STRING)
                req->policies_contents.push_back(policies_contents->array[i].s);
    }
    if (display_name && display_name->type == Container::Value::CONTAINER_TYPE_STRING)
        req->display_name = display_name->s;
    if (display_content && display_content->type == Container::Value::CONTAINER_TYPE_STRING)
        req->display_content = display_content->s;

    if (verbosity && verbosity->type == Container::Value::CONTAINER_TYPE_INTEGER)
    {
        req->has_verbosity = true;
        req->verbosity = verbosity->l;
    }

    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Retry_Req *RESTAPI::parse_retry_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "RETRY");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ids;
    ids = model->get_value_by_key(*child, "ids");

    if (!ids || ids->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return NULL;

    Retry_Req *req = new Retry_Req;
    for (size_t i = 0; i < ids->array.size(); ++i)
    {
        Container::Value *id = &ids->array[i];

        if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        {
            delete req;
            return NULL;
        }
        req->ids.push_back(id->l);
    }
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Clear_Req *RESTAPI::parse_clear_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "CLEAR");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ids;
    ids = model->get_value_by_key(*child, "ids");

    if (!ids || ids->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return NULL;

    Clear_Req *req = new Clear_Req;
    for (size_t i = 0; i < ids->array.size(); ++i)
    {
        Container::Value *id = &ids->array[i];

        if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        {
            delete req;
            return NULL;
        }
        req->ids.push_back(id->l);
    }
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::List_Req *RESTAPI::parse_list_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "LIST");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;
    List_Req *req = new List_Req;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Validate_Req *RESTAPI::parse_validate_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "VALIDATE");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ids, *report, *policies_names, *policies_contents;
    ids = model->get_value_by_key(*child, "ids");
    report = model->get_value_by_key(*child, "report");
    policies_names = model->get_value_by_key(*child, "policies_names");
    policies_contents = model->get_value_by_key(*child, "policies_contents");

    if (!ids || !report || ids->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return NULL;

    Validate_Req *req = new Validate_Req;
    for (size_t i = 0; i < ids->array.size(); ++i)
    {
        Container::Value *id = &ids->array[i];

        if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        {
            delete req;
            return NULL;
        }
        req->ids.push_back(id->l);
    }

    if (report->type != Container::Value::CONTAINER_TYPE_STRING)
    {
        delete req;
        return NULL;
    }
    req->report = string_to_Report(report->s);

    if (policies_names && policies_names->type == Container::Value::CONTAINER_TYPE_ARRAY)
    {
        for (size_t i = 0; i < policies_names->array.size(); ++i)
            if (policies_names->array[i].type == Container::Value::CONTAINER_TYPE_STRING)
                req->policies_names.push_back(policies_names->array[i].s);
    }
    if (policies_contents && policies_contents->type == Container::Value::CONTAINER_TYPE_ARRAY)
    {
        for (size_t i = 0; i < policies_contents->array.size(); ++i)
            if (policies_contents->array[i].type == Container::Value::CONTAINER_TYPE_STRING)
                req->policies_contents.push_back(policies_contents->array[i].s);
    }

    return req;
}

//---------------------------------------------------------------------------
RESTAPI::File_From_Id_Req *RESTAPI::parse_file_from_id_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "FILE_FROM_ID");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *id;
    id = model->get_value_by_key(*child, "id");
    if (!id || id->type != Container::Value::CONTAINER_TYPE_INTEGER)
        return NULL;

    File_From_Id_Req *req = new File_From_Id_Req;
    req->id = id->l;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Default_Values_For_Type_Req *RESTAPI::parse_default_values_for_type_req(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "DEFAULT_VALUES_FOR_TYPE");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *type = model->get_value_by_key(*child, "type");
    if (!type || type->type != Container::Value::CONTAINER_TYPE_STRING)
        return NULL;

    Default_Values_For_Type_Req *req = new Default_Values_For_Type_Req;
    req->type = type->s;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Analyze_Req *RESTAPI::parse_uri_analyze_req(const std::string&)
{
    Analyze_Req *req = new Analyze_Req;
    //TODO
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Status_Req *RESTAPI::parse_uri_status_req(const std::string& uri)
{
    Status_Req *req = new Status_Req;

    size_t end, start = 0;
    while (1)
    {
        end = uri.find("=", start);
        if (end == std::string::npos || uri.substr(start, end - start) != "id")
            goto finish;
        start = end + 1;

        end = uri.find("&", start);

        std::string id = uri.substr(start, end - start);
        if (!id.length())
            goto finish;

        req->ids.push_back(strtoll(id.c_str(), NULL, 10));
        if (end == std::string::npos)
            goto finish;
        start = end + 1;
    }

finish:
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Report_Req *RESTAPI::parse_uri_report_req(const std::string&)
{
    Report_Req *req = new Report_Req;
    //TODO
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Retry_Req *RESTAPI::parse_uri_retry_req(const std::string&)
{
    Retry_Req *req = new Retry_Req;
    //TODO
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Clear_Req *RESTAPI::parse_uri_clear_req(const std::string& uri)
{
    Clear_Req *req = new Clear_Req;
    size_t end, start = 0;
    while (1)
    {
        end = uri.find("=", start);
        if (end == std::string::npos || uri.substr(start, end - start) != "id")
            goto finish;
        start = end + 1;

        end = uri.find("&", start);

        std::string id = uri.substr(start, end - start);
        if (!id.length())
            goto finish;

        req->ids.push_back(strtoll(id.c_str(), NULL, 10));
        if (end == std::string::npos)
            goto finish;
        start = end + 1;
    }

finish:
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::List_Req *RESTAPI::parse_uri_list_req(const std::string&)
{
    List_Req *req = new List_Req;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Validate_Req *RESTAPI::parse_uri_validate_req(const std::string&)
{
    Validate_Req *req = new Validate_Req;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::File_From_Id_Req *RESTAPI::parse_uri_file_from_id_req(const std::string&)
{
    File_From_Id_Req *req = new File_From_Id_Req;
    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Default_Values_For_Type_Req *RESTAPI::parse_uri_default_values_for_type_req(const std::string& uri)
{
    Default_Values_For_Type_Req *req = new Default_Values_For_Type_Req;

    size_t end, start = 0;
    end = uri.find("=", start);
    if (end == std::string::npos || uri.substr(start, end - start) != "type")
        return req;

    start = end + 1;
    end = uri.find("&", start);

    std::string type = uri.substr(start, end - start);
    if (!type.length())
        return req;

    req->type = type;

    return req;
}

//---------------------------------------------------------------------------
RESTAPI::Analyze_Res *RESTAPI::parse_analyze_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "ANALYZE_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Analyze_Res *res = new Analyze_Res;

    if (ok && parse_analyze_ok(ok, res->ok))
    {
        delete res;
        return NULL;
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Analyze_Nok *tmp = new Analyze_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Status_Res *RESTAPI::parse_status_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "STATUS_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Status_Res *res = new Status_Res;
    if (ok && parse_status_ok(ok, res->ok))
    {
        delete res;
        return NULL;
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Status_Nok *tmp = new Status_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Report_Res *RESTAPI::parse_report_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "REPORT_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Report_Res *res = new Report_Res;
    if (ok && parse_report_ok(ok, res->ok))
    {
        delete res;
        return NULL;
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Report_Nok *tmp = new Report_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Retry_Res *RESTAPI::parse_retry_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "RETRY_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Retry_Res *res = new Retry_Res;
    if (ok)
    {
        if (ok->type != Container::Value::CONTAINER_TYPE_ARRAY)
        {
            delete res;
            return NULL;
        }
        for (size_t i = 0; i < ok->array.size(); ++i)
        {
            Container::Value *tmp = &ok->array[i];

            if (tmp->type != Container::Value::CONTAINER_TYPE_INTEGER)
            {
                delete res;
                return NULL;
            }
            res->ok.push_back(tmp->l);
        }
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Retry_Nok *tmp = new Retry_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Clear_Res *RESTAPI::parse_clear_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "CLEAR_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Clear_Res *res = new Clear_Res;
    if (ok)
    {
        if (ok->type != Container::Value::CONTAINER_TYPE_ARRAY)
        {
            delete res;
            return NULL;
        }
        for (size_t i = 0; i < ok->array.size(); ++i)
        {
            Container::Value *tmp = &ok->array[i];

            if (tmp->type != Container::Value::CONTAINER_TYPE_INTEGER)
            {
                delete res;
                return NULL;
            }
            res->ok.push_back(tmp->l);
        }
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Clear_Nok *tmp = new Clear_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::List_Res *RESTAPI::parse_list_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "LIST_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *files;
    files = model->get_value_by_key(*child, "files");

    List_Res *res = new List_Res;
    if (files->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < files->array.size(); ++i)
    {
        List_File *tmp = new List_File;

        if (parse_list_file(&files->array[i], tmp->file, tmp->id))
        {
            delete res;
            return NULL;
        }
        res->files.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Validate_Res *RESTAPI::parse_validate_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "VALIDATE_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *ok, *nok;
    ok = model->get_value_by_key(*child, "ok");
    nok = model->get_value_by_key(*child, "nok");

    Validate_Res *res = new Validate_Res;
    if (ok && parse_validate_ok(ok, res->ok))
    {
        delete res;
        return NULL;
    }

    if (!nok)
        return res;

    if (nok->type != Container::Value::CONTAINER_TYPE_ARRAY)
    {
        delete res;
        return NULL;
    }

    for (size_t i = 0; i < nok->array.size(); ++i)
    {
        Validate_Nok *tmp = new Validate_Nok;

        if (parse_generic_nok(&nok->array[i], tmp->id, tmp->error))
        {
            delete res;
            return NULL;
        }
        res->nok.push_back(tmp);
    }
    return res;
}

//---------------------------------------------------------------------------
RESTAPI::File_From_Id_Res *RESTAPI::parse_file_from_id_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "FILE_FROM_ID_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *file;
    file = model->get_value_by_key(*child, "file");

    if (!file || file->type != Container::Value::CONTAINER_TYPE_STRING)
        return NULL;

    File_From_Id_Res *res = new File_From_Id_Res;
    res->file = file->s;

    return res;
}

//---------------------------------------------------------------------------
RESTAPI::Default_Values_For_Type_Res *RESTAPI::parse_default_values_for_type_res(const std::string& data)
{
    Container::Value v, *child;

    if (model->parse(data, v))
    {
        error = model->get_error();
        return NULL;
    }

    child = model->get_value_by_key(v, "DEFAULT_VALUES_FOR_TYPE_RESULT");
    if (!child || child->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return NULL;

    Container::Value *values;
    values = model->get_value_by_key(*child, "values");

    if (!values || values->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return NULL;

    Default_Values_For_Type_Res *res = new Default_Values_For_Type_Res;
    for (size_t i = 0; i < values->array.size(); ++i)
    {
        if (values->array[i].type != Container::Value::CONTAINER_TYPE_STRING)
            continue;

        res->values.push_back(values->array[i].s);
    }

    return res;
}

//***************************************************************************
// HELPER
//***************************************************************************

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_analyze_args(std::vector<Analyze_Arg>& args)
{
    Container::Value args_val;

    args_val.type = Container::Value::CONTAINER_TYPE_ARRAY;

    for (size_t i = 0; i < args.size(); ++i)
    {
        Container::Value arg, file, id, force;
        arg.type = Container::Value::CONTAINER_TYPE_OBJECT;

        file.type = Container::Value::CONTAINER_TYPE_STRING;
        file.s = args[i].file;
        arg.obj["file"] = file;

        id.type = Container::Value::CONTAINER_TYPE_INTEGER;
        id.l = args[i].id;
        arg.obj["id"] = id;

        if (args[i].has_force_analyze)
        {
            force.type = Container::Value::CONTAINER_TYPE_BOOL;
            force.b = args[i].force_analyze;
            arg.obj["force"] = force;
        }

        args_val.array.push_back(arg);
    }

    return args_val;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_ids(std::vector<int>& ids)
{
    Container::Value array;

    array.type = Container::Value::CONTAINER_TYPE_ARRAY;

    for (size_t i = 0; i < ids.size(); ++i)
    {
        Container::Value arg, id;
        id.type = Container::Value::CONTAINER_TYPE_INTEGER;
        id.l = ids[i];
        array.array.push_back(id);
    }

    return array;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_report_reports(std::vector<Report>& reports)
{
    Container::Value reports_val;

    reports_val.type = Container::Value::CONTAINER_TYPE_ARRAY;

    for (size_t i = 0; i < reports.size(); ++i)
    {
        Container::Value report;

        report.type = Container::Value::CONTAINER_TYPE_STRING;
        report.s = get_Report_string(reports[i]);

        reports_val.array.push_back(report);
    }

    return reports_val;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_report_string(const std::string& reports)
{
    Container::Value str_val;

    str_val.type = Container::Value::CONTAINER_TYPE_STRING;
    str_val.s = reports;

    return str_val;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_report_int(int val)
{
    Container::Value int_val;

    int_val.type = Container::Value::CONTAINER_TYPE_INTEGER;
    int_val.l = val;

    return int_val;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_report_arr_str(const std::vector<std::string>& reports)
{
    Container::Value arr_val;

    arr_val.type = Container::Value::CONTAINER_TYPE_ARRAY;
    for (size_t i = 0; i < reports.size(); ++i)
    {
        Container::Value str_val;

        str_val.type = Container::Value::CONTAINER_TYPE_STRING;
        str_val.s = reports[i];
        arr_val.array.push_back(str_val);
    }

    return arr_val;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_generic_nok(int id, Reason error)
{
    Container::Value nok, id_v, error_v;

    nok.type = Container::Value::CONTAINER_TYPE_OBJECT;

    id_v.type = Container::Value::CONTAINER_TYPE_INTEGER;
    id_v.l = id;
    nok.obj["id"] = id_v;

    error_v.type = Container::Value::CONTAINER_TYPE_STRING;
    error_v.s = get_Reason_string(error);
    nok.obj["error"] = error_v;
    return nok;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_analyze_oks(std::vector<Analyze_Ok*>& array)
{
    Container::Value ok;

    ok.type = Container::Value::CONTAINER_TYPE_ARRAY;

    for (size_t i = 0; i < array.size(); ++i)
    {
        if (!array[i])
            continue;

        Container::Value v, in, out, create;

        v.type = Container::Value::CONTAINER_TYPE_OBJECT;

        in.type = Container::Value::CONTAINER_TYPE_INTEGER;
        in.l = array[i]->inId;
        v.obj["inId"] = in;

        out.type = Container::Value::CONTAINER_TYPE_INTEGER;
        out.l = array[i]->outId;
        v.obj["outId"] = out;

        create.type = Container::Value::CONTAINER_TYPE_BOOL;
        create.b = array[i]->create;
        v.obj["create"] = create;

        ok.array.push_back(v);
    }

    return ok;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_status_oks(std::vector<Status_Ok*>& array)
{
    Container::Value ok;

    ok.type = Container::Value::CONTAINER_TYPE_ARRAY;

    for (size_t i = 0; i < array.size(); ++i)
    {
        if (!array[i])
            continue;

        Container::Value v, id, finished, done, tool;

        v.type = Container::Value::CONTAINER_TYPE_OBJECT;

        id.type = Container::Value::CONTAINER_TYPE_INTEGER;
        id.l = array[i]->id;
        v.obj["id"] = id;

        finished.type = Container::Value::CONTAINER_TYPE_BOOL;
        finished.b = array[i]->finished;
        v.obj["finished"] = finished;

        if (array[i]->has_percent)
        {
            done.type = Container::Value::CONTAINER_TYPE_REAL;
            done.d = array[i]->done;
            v.obj["done"] = done;
        }

        if (array[i]->finished && array[i]->has_tool)
        {
            tool.type = Container::Value::CONTAINER_TYPE_INTEGER;
            tool.l = array[i]->tool;
            v.obj["tool"] = tool;
        }

        ok.array.push_back(v);
    }

    return ok;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_report_ok(Report_Ok& obj)
{
    Container::Value ok;

    ok.type = Container::Value::CONTAINER_TYPE_OBJECT;
    Container::Value report, valid;

    if (obj.report.length() > 0)
    {
        report.type = Container::Value::CONTAINER_TYPE_STRING;
        report.s = obj.report;
        ok.obj["report"] = report;
    }

    if (obj.has_valid)
    {
        valid.type = Container::Value::CONTAINER_TYPE_BOOL;
        valid.b = obj.valid;
        ok.obj["valid"] = valid;
    }
    return ok;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_list_file(const std::string& filename, int id)
{
    Container::Value file, id_v, filename_v;

    file.type = Container::Value::CONTAINER_TYPE_OBJECT;

    id_v.type = Container::Value::CONTAINER_TYPE_INTEGER;
    id_v.l = id;
    file.obj["id"] = id_v;

    filename_v.type = Container::Value::CONTAINER_TYPE_STRING;
    filename_v.s = filename;
    file.obj["file"] = filename_v;
    return file;
}

//---------------------------------------------------------------------------
Container::Value RESTAPI::serialize_validate_ok(Validate_Ok* obj)
{
    Container::Value ok;
    ok.type = Container::Value::CONTAINER_TYPE_OBJECT;

    if (!obj)
        return ok;

    Container::Value id, valid;

    id.type = Container::Value::CONTAINER_TYPE_INTEGER;
    id.l = obj->id;
    ok.obj["id"] = id;

    valid.type = Container::Value::CONTAINER_TYPE_BOOL;
    valid.b = obj->valid;
    ok.obj["valid"] = valid;

    return ok;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_analyze_arg(Container::Value *v, std::vector<Analyze_Arg>& args)
{
    if (v->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return -1;

    for (size_t i = 0; i < v->array.size(); ++i)
    {
        Container::Value *obj = &v->array[i];

        if (obj->type != Container::Value::CONTAINER_TYPE_OBJECT)
            return -1;

        Container::Value *file, *id, *force;

        file = model->get_value_by_key(*obj, "file");
        id = model->get_value_by_key(*obj, "id");
        force = model->get_value_by_key(*obj, "force");

        if (!file || !id || file->type != Container::Value::CONTAINER_TYPE_STRING ||
            id->type != Container::Value::CONTAINER_TYPE_INTEGER)
            return -1;

        Analyze_Arg arg;
        arg.file = file->s;
        arg.id = id->l;

        if (force && force->type == Container::Value::CONTAINER_TYPE_BOOL)
        {
            arg.has_force_analyze = true;
            arg.force_analyze = force->b;
        }

        args.push_back(arg);
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_report_reports(Container::Value *v, std::vector<Report>& reports)
{
    if (v->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return -1;

    for (size_t i = 0; i < v->array.size(); ++i)
    {
        Container::Value *obj = &v->array[i];

        if (obj->type != Container::Value::CONTAINER_TYPE_STRING)
            return -1;

        reports.push_back(string_to_Report(obj->s));
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_generic_nok(Container::Value *v, int& id, Reason& error)
{
    if (v->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return -1;

    Container::Value *id_v, *error_v;

    id_v = model->get_value_by_key(*v, "id");
    error_v = model->get_value_by_key(*v, "error");

    if (!id_v || id_v->type != Container::Value::CONTAINER_TYPE_INTEGER ||
        !error_v || error_v->type != Container::Value::CONTAINER_TYPE_STRING)
        return -1;

    id = id_v->l;
    error = string_to_Reason(error_v->s);
    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_analyze_ok(Container::Value *v, std::vector<Analyze_Ok*>& oks)
{
    if (v->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return -1;

    for (size_t i = 0; i < v->array.size(); ++i)
    {
        Container::Value *obj = &v->array[i];

        if (obj->type != Container::Value::CONTAINER_TYPE_OBJECT)
            return -1;

        Container::Value *inId, *outId, *create;

        inId = model->get_value_by_key(*obj, "inId");
        outId = model->get_value_by_key(*obj, "outId");
        create = model->get_value_by_key(*obj, "create");

        if (!inId || inId->type != Container::Value::CONTAINER_TYPE_INTEGER ||
            !outId || outId->type != Container::Value::CONTAINER_TYPE_INTEGER)
            return -1;

        Analyze_Ok *ok = new Analyze_Ok;
        ok->inId = inId->l;
        ok->outId = outId->l;
        if (create && create->type == Container::Value::CONTAINER_TYPE_BOOL)
            ok->create = create->b;

        oks.push_back(ok);
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_status_ok(Container::Value *v, std::vector<Status_Ok*>& oks)
{
    if (v->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return -1;

    for (size_t i = 0; i < v->array.size(); ++i)
    {
        Container::Value *obj = &v->array[i];

        if (obj->type != Container::Value::CONTAINER_TYPE_OBJECT)
            return -1;

        Container::Value *id, *finished, *done, *tool;

        id = model->get_value_by_key(*obj, "id");
        finished = model->get_value_by_key(*obj, "finished");
        done = model->get_value_by_key(*obj, "done");
        tool = model->get_value_by_key(*obj, "tool");

        if (!id || id->type != Container::Value::CONTAINER_TYPE_INTEGER ||
            !finished || finished->type != Container::Value::CONTAINER_TYPE_BOOL)
            return -1;

        Status_Ok *ok = new Status_Ok;
        ok->id = id->l;
        ok->finished = finished->b;

        if (!done)
            ok->has_percent = false;
        else if (done->type == Container::Value::CONTAINER_TYPE_REAL)
        {
            ok->has_percent = true;
            ok->done = done->d;
        }
        else if (done->type == Container::Value::CONTAINER_TYPE_INTEGER)
        {
            ok->has_percent = true;
            ok->done = (double)done->l;
        }
        else
            return -1;

        if (tool && tool->type == Container::Value::CONTAINER_TYPE_INTEGER)
        {
            ok->has_tool = true;
            ok->tool = (Report)tool->l;
        }
        else
            ok->has_tool = false;

        oks.push_back(ok);
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_report_ok(Container::Value *v, Report_Ok& ok)
{
    if (v->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return -1;

    Container::Value *report, *valid;

    report = model->get_value_by_key(*v, "report");
    valid = model->get_value_by_key(*v, "valid");

    if (report)
    {
        if (report->type != Container::Value::CONTAINER_TYPE_STRING)
            return -1;
        ok.report = report->s;
    }

    if (valid)
    {
        if (valid->type != Container::Value::CONTAINER_TYPE_BOOL)
            return -1;
        ok.has_valid = true;
        ok.valid = valid->b;
    }

    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_list_file(Container::Value *v, std::string& file, int& id)
{
    if (v->type != Container::Value::CONTAINER_TYPE_OBJECT)
        return -1;

    Container::Value *id_v, *file_v;

    file_v = model->get_value_by_key(*v, "file");
    id_v = model->get_value_by_key(*v, "id");

    if (!id_v || id_v->type != Container::Value::CONTAINER_TYPE_INTEGER ||
        !file_v || file_v->type != Container::Value::CONTAINER_TYPE_STRING)
        return -1;

    id = id_v->l;
    file = file_v->s;
    return 0;
}

//---------------------------------------------------------------------------
int RESTAPI::parse_validate_ok(Container::Value *v, std::vector<Validate_Ok*>& oks)
{
    if (v->type != Container::Value::CONTAINER_TYPE_ARRAY)
        return -1;

    for (size_t i = 0; i < v->array.size(); ++i)
    {
        Validate_Ok* ok = new Validate_Ok;
        Container::Value *id, *valid;
        id = model->get_value_by_key(v->array[i], "id");
        valid = model->get_value_by_key(v->array[i], "valid");

        if (id)
        {
            if (id->type != Container::Value::CONTAINER_TYPE_INTEGER)
                return -1;
            ok->id = id->l;
        }

        if (valid)
        {
            if (valid->type != Container::Value::CONTAINER_TYPE_BOOL)
                return -1;
            ok->valid = valid->b;
        }
        oks.push_back(ok);
    }

    return 0;
}

}
