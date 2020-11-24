#ifndef RECORD_H
#define  RECORD_H
#include <string>

namespace ispecid{ namespace datatypes{
    
    class record{
        private:
        std::string species_name;
        std::string cluster;
        std::string source;
        std::string grade;
        int size;

        public:
        record(std::string _species_name, std::string _cluster, std::string _source, std::string _grade, int _size = 1):
            species_name(_species_name), cluster(_cluster), source(_source), grade(_grade), size(_size)
        {
        };

        static std::string getSpeciesName(record& record){
            return record.getSpeciesName();
        }

        std::string getSpeciesName(){
            return species_name;
        }
        std::string getCluster(){
            return cluster;
        }
        std::string getSource(){
            return source;
        }
        std::string getGrade(){
            return grade;
        }        
        int count(){
            return size;
        }
        void setSpeciesName(std::string _species_name){
            this->species_name = _species_name;
        }
        void setCluster(std::string _cluster){
            this->cluster = _cluster;
        }
        void setSource(std::string _source){
            this->source = _source;
        }
        void setGrade(std::string _grade){
            this->grade = _grade;
        }
        void addCount(int _size){
            size += _size;
        }
        static bool goodRecord(record& record){
            return record.species_name.empty() || record.cluster.empty() || record.source.empty();
        }
        std::string toString(){
            return species_name + ";" + cluster+ ";" +source+ ";" + grade;
        }
        
    };
}}
#endif