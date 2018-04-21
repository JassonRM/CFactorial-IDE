//
// Created by marco on 27/03/18.
//

#include "Json.h"
#include "Requests.h"
#include <stdlib.h>
void Json::put(string key,string value){
    QJsonValue *jsonValue = new QJsonValue(QString::fromStdString(value));
    this->json->insert(QString::fromStdString(key),*jsonValue);
    delete(jsonValue);
}
void Json::put(string key,int value){
    QJsonValue *jsonValue = new QJsonValue(value);
    this->json->insert(QString::fromStdString(key),*jsonValue);
    delete(jsonValue);
}

void Json::put(string key,double value){
    QJsonValue *jsonValue = new QJsonValue(value);
    this->json->insert(QString::fromStdString(key),*jsonValue);
    delete(jsonValue);
}

void Json::put(string key,float value){
    QJsonValue *jsonValue = new QJsonValue(value);
    this->json->insert(QString::fromStdString(key),*jsonValue);
    delete(jsonValue);
}

void Json::put(string key,long value){
    QJsonValue *jsonValue = new QJsonValue(qint64(value));
    this->json->insert(QString::fromStdString(key),*jsonValue);
    delete(jsonValue);
}

void Json::putJsonValue(string key,QJsonValue value){
    this->json->insert(QString::fromStdString(key),value);
}


string Json::addValue(string value, Token* token){
    if (this->get("Struct") == "true"){
        if (token->type != IDENTIFIER || !Requests::isVariable(value)
                || Requests::variableType(value) != this->get("Type")
                || this->get("Value") != ""){
            //error
            return "ERROR";
        }
        this->put("Value", value);
        return " ";

    }
    string response = " ";
    if (token->type == IDENTIFIER){
        Json* var = Requests::variableValue(token->value);

        if(this->get("Type") == ""){
            this->put("Type",var->get("Type"));
        }

        if (var->get("Value") == ""){
            response = "variable doesn't exist";
            return response;
        }
        else if (var->get("Type") != this->get("Type")){
            response = "types don't match";
            return response;
        }
        value = var->get("Value");
    }else if (token->type == LITERAL) {

        if (this->get("Type") == "long") {
            try {
                boost::lexical_cast<long>(value);
            } catch (boost::bad_lexical_cast) {
                response = "Error in line" + to_string(token->line) + " : Variable \"" + this->get("Identifier") +
                           "\" is not of type long\n";
                return response;
            }
        } else if (this->get("Type") == "int") {
            try {
                boost::lexical_cast<int>(value);
            } catch (boost::bad_lexical_cast) {
                response = "Error in line" + to_string(token->line) + " : Variable \"" + this->get("Identifier") +
                           "\" is not of type int\n";
                return response;
            }
        } else if (this->get("Type") == "float") {
            try {
                boost::lexical_cast<float>(value);
            } catch (boost::bad_lexical_cast) {
                response = "Error in line" + to_string(token->line) + " : Variable \"" + this->get("Identifier") +
                           "\" is not of type float\n";
                return response;
            }
        } else if (this->get("Type") == "double") {
            try {
                boost::lexical_cast<double>(value);
            } catch (boost::bad_lexical_cast) {
                response = "Error in line" + to_string(token->line) + " : Variable \"" + this->get("Identifier") +
                           "\" is not of type double\n";
                return response;
            }
        } else if (this->get("Type") == "char") {
            if (!regex_match(value, regex("\'.\'"))) {
                response = "Error in line" + to_string(token->line) + " : Variable \"" + this->get("Identifier") +
                           "\" is not of type char\n";
                return response;
            }

        }
    }else if (token->type == OPERATOR) {
            string type = this->get("Type");
            if(this->get("Value") == ""){
                return "ERROR";
            }
            if (type != "int" && type != "double" && type != "float" && type != "long" && type != "") {
                response = "Error in line" + to_string(token->line) + " : Can't use <" + token->value +
                           "> operator with non numeric data type\n";
                return response;
            }
    }
    this->put("Value", this->get("Value").append(value));
    return response;
}

QJsonObject* Json::get(){
    return this->json;
}

string Json::get(string key){

    const QJsonValue &jsonValue = json->value(QString::fromStdString(key));
    if (jsonValue.isUndefined()){
        return "";
    }
    return jsonValue.toString().toStdString();
}

string Json::toString(){
    QJsonDocument doc(*this->json);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    return strJson.toStdString();
}

void Json::submit() {
    if(this->get("Request")== "Change Value"){
        cout <<this->toString()<<endl;
        return;
    }
    this->put("Request", "New Variable");
    cout <<this->toString()<<endl;
}

string Json::addValue(string value, tokenType type){
    Token* token = new Token(value,type);
    string response = addValue(value,token);
    delete(token);
    return response;
}

string Json::addValueUnchecked(string value){
    this->put("Value", this->get("Value").append(value));
}

void Json::arithmeticSolver(){

    if (this->get("Type") == "int"){
        if (this->get("Value") != ""){
            int value = (int)te_interp(this->get("Value").data(),0);
            this->put("Value",value);
        }
    }
    else if (this->get("Type") == "double"){
        if (this->get("Value") != ""){
            double value = boost::lexical_cast<double>(this->get("Value"));
            this->put("Value",value);
        }
    }
    else if (this->get("Type") == "float"){
        if (this->get("Value") != ""){
            float value = std::stof(this->get("Value"));
            cout <<this->get("Value");
            this->put("Value",value);
        }
    }
    else if (this->get("Type") == "long"){
        if (this->get("Value") != ""){
            long value = (long)te_interp(this->get("Value").data(),0);
            this->put("Value",value);
        }
    }else{};
}