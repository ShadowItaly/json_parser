#ifndef GC_JSON_PARSER_HPP
#define GC_JSON_PARSER_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
/**
 * @author Alexander Leonhardt.
 */

/**
 * @brief Describes the JsonTypes that are possible at the moment
 */
enum class JsonType {
  object,       ///< An Json object, contains key : Json pairs, empty keys are forbidden
  array,        ///< An Json array contains a list of Jsons
  integer,      ///< An Json integer, contains a normal integer
  string,       ///< An Json string, contains a normal json string
  boolean,      ///< An Json boolean, contains a normal json boolean
  null,         ///< A Json null value.
  floating_point,       ///< A Json double value.
};

/**
 * @brief The possible log levels of the json implementation
 */
enum class JsonLogLevel {
  none = 0,   ///< Log nothing at all
  log_error,  ///< Log only errors to the provided logging functor
  log_trace   ///< Log trace information to the logging functor
};

/**
 * @brief Describes the JsonErrors
 */
enum class JsonError {
  ok,                 ///< No error occured
  not_implemented,    ///< The function that was called was not implemented
  does_not_exist,     ///< The key that was searched for does not exist
  empty_attribute_key,///< Provided an empty attribute key to the json object operation insert
  parse_error,        ///< Parsing error occured and the returned json is flawed!
};

/**
 * @brief A basic implementation of an Stdout functor, will print to stdout all
 * logging informations
 */
struct JsonStdoutFunctor {
  /**
   * @brief Functor implementation will print 
   *
   * @param x
   */
  JsonStdoutFunctor(std::string x) {
    std::cout<<x<<std::endl;
  }
};

/**
 * @brief A basic implementation which prints all json output in red
 */
struct JsonStdoutColoredFunctor {
  /**
   * @brief Prints the given string via std::cout to stdout. Only works in
   * terminals which support escape color sequences.
   *
   * @param x The string to print to stdout in red.
   */
   JsonStdoutColoredFunctor(std::string x) {
    std::cout<<"\033[1;31m"<<x<<"\033[0m;"<<std::endl;
  }
};

/**
 * @brief Describes the JsonBase class contains all helper functions to deal
 * with the underlying interface.
 *
 * @tparam log_level The log level at the moment unused TODO: Implement this
 * feature
 * @tparam log_functor The function to log results at the moment unused TODO:
 * Implement this feature
 */
template<JsonLogLevel log_level=JsonLogLevel::none, typename log_functor=JsonStdoutColoredFunctor>
class JsonBase {
  public:
    /**
     * @brief The Basic interface to interact with, can be Object, Array,
     * String, Integer or Boolean, therefore it provides functions for all of
     * these implementations
     */
    class JsonInterface {
      public:
        /**
         * @brief Returns the type of the underlying implementation can be
         * anything of JsonType.
         *
         * @return The type of the implementation
         */
        virtual JsonType type() = 0;

        /**
         * @brief Returns the size of the implementation is 1 for all primitive
         * data types or the container size for more complex types
         *
         * @return The size of the underlying container or 1 for a primitive
         * type.
         */
        virtual int size() = 0;
        
        /**
         * @brief Inserts the key into the underlying container, illegal
         * operation for all primitive data types. The standard definition
         * states not implemented to reduce the boilerplate amount
         *
         * @param key The key must be "" for an array and must be non empty for
         * an object insertion
         * @param new_insert The new Json to insert into the container
         * @param err A reference to the error will fill the error if an error
         * occured, does not change the err otherwise
         */
        virtual void insert(const std::string &key, JsonBase *new_insert, JsonError &err) {
          err = JsonError::not_implemented;
        }

        /**
         * @brief Returns the JsonBase for the given key, this operation is only
         * valid for json objects. For unimplemented cases sets the err field to
         * JsonError::not_implemented and returns an nullptr
         *
         * @param key The key to search for in the object
         * @param err A reference to an error, will fill the error if an error
         * occurs and does not touch the variable otherwise
         *
         * @return The found JsonObject or nullptr if there is no object with
         * this key name
         */
        virtual JsonBase* get(const std::string &key, JsonError &err) {
          err = JsonError::not_implemented;
          return nullptr;
        }

        /**
         * @brief Return the Json at the given base index, the interface does
         * not require out of bound checking! This function is implemented in
         * json array types. For unimplemented cases set the err field to
         * JsonError::not_implemented and return nullptr
         *
         * @param index The index to access.
         * @param err A reference to an error instance, the function will set
         * the error if one occurs and does not touch the error otherwise
         *
         * @return 
         */
        virtual JsonBase* get(const int index, JsonError &err) {
          err = JsonError::not_implemented;
          return nullptr;
        }
        
        /**
         * @brief Converts the json into a string. 
         *
         * @return The json converted to string format
         */
        virtual std::string dump() = 0;

        /**
         * @brief Returns the stored string is only implemented for
         * JsonType::string. For unimplemented cases set the err field to
         * JsonError::unimplemented and return an empty string
         *
         * @param err Fills the error if the underlying type is not of type
         * JsonType::string 
         *
         * @return Returns the contained string if the type fits.
         */
        virtual std::string to_string(JsonError &err) {
          err = JsonError::not_implemented;
          return "";
        }

        /**
         * @brief Returns the stored int if the underlying type is integer, for
         * unimplemented cases set error field to JsonError::not_implemented and
         * return -1.
         *
         * @param err A reference to an error, the error gets filled if the
         * undelrying type is not JsonType::integer. 
         *
         * @return Return the stored integer if there is any
         */
        virtual long long to_int(JsonError &err) {
          err = JsonError::not_implemented;
          return -1;
        }
        
        /**
         * @brief Returns the stored boolean if the underlying type is boolean,
         * for unimplemented cases set the error field to
         * JsonError::not_implemented and return false.
         *
         * @param err A reference to an error instance, it is filled if the
         * underlying type is not JsonType::boolean
         *
         * @return Returns the stored boolean if there is any
         */
        virtual bool to_bool(JsonError &err) {
          err = JsonError::not_implemented;
          return false;
        }

        /**
         * @brief Returns the stored double value if the underlying type is
         * boolean, for unimplemented cases set the error field to
         * JsonError::not_implemented and return 0
         *
         * @param err A reference to an error instance, it if filled if the
         * underlying type is not JsonType::double
         *
         * @return  Returns the stored double or 0 if it is not an double
         */
        virtual double to_double(JsonError &err) {
          err = JsonError::not_implemented;
          return 0;
        }

        /**
         * @brief Creates an empty destructor to make sure that the inheriting
         * implementations can call their constructor instead
         */
        virtual ~JsonInterface() {};
    };

    /**
     * @brief The implementation of a json object, implements the necesarry
     * functions of JsonInterface and feeds an error to the unsupported
     * functions
     */
    class JsonImplObject : public JsonInterface {
      public:
        /**
         * @brief Returns the type of the Implementation, is always
         * JsonType::object for this class
         *
         * @return Returns JsonType::object.
         */
        JsonType type() override {
          return JsonType::object;
        }

        /**
         * @brief Returns the size of the json container, is always the size of
         * the container.
         *
         * @return Returns the number of stored attributes.
         */
        int size() override {
          return traits_.size();
        }

        /**
         * @brief Inserts a new attribute with the given key into the map if the
         * key exists overwrite the existent key and delete the stored object.
         *
         * @param key The key to insert/overrwrite inside the map
         * @param new_insert The object to include into the map, this function
         * takes memory ownership. This means the object gets deleted by the
         * destructor of this class. Make sure there are no multiple ownerships.
         * @param err A reference to an error instance, this puts
         * JsonError::empty_attribute_key inside the error if the key is empty
         */
        void insert(const std::string &key, JsonBase *new_insert, JsonError &err) override {
          // Check if key is non empty
          if (key=="")
            err = JsonError::empty_attribute_key;
          else {
            // Ok key is nonempty swap the key inside the map, if the key does
            // not exist yet, the map will put nullptr inside new_insert
            std::swap(traits_[key],new_insert);
            
            // If there was an object with that key stored before delete the
            // object
            if (new_insert)
              delete new_insert;
          }
        }

        /**
         * @brief Returns the Json associated with the given key, if the key
         * does not exist returns nullptr
         *
         * @param key The key to look for inside the map
         * @param err The error will get filled when accessing an element that
         * does not exist with JsonError::does_not_exist
         *
         * @return The Json object if found or nullptr otherwise
         */
        JsonBase* get(const std::string &key, JsonError &err) override {
          auto fnd = traits_.find(key);
          if (fnd == traits_.end()) {
            err = JsonError::does_not_exist;
            return nullptr;
          }
          return fnd->second;
        }

        /**
         * @brief Converts the json object into a string and returns the string
         *
         * @return 
         */
        std::string dump() override {
          // Check if the json object is empty if so return the equivalent of an
          // empty json
          if(traits_.size() == 0)
            return "{}";

          // Ok the json is not empty open the beginning brackets and dump every
          // attribute inside the json
          std::string ret = "{";
          for (const auto &it : traits_) {
            ret += "\""+it.first+"\":";
            ret += std::move(it.second->dump());

            // Always put an , after every attribute it will come in handy in
            // the end
            ret += ",";
          }
          // Overwrite the last , as the map was not empty we will have at least
          // one iteration in the loop and always an , at the end of the string.
          // The size of the string does not change so we can do this operation
          // without worrying much.
          ret[ret.length()-1] = '}';
          return std::move(ret);
        }

        /**
         * @brief This is actually a non derived function and executes the
         * function func on every pair inside the map.
         *
         * @param func The function to apply on each element in the json object
         */
        void map_for_each(std::function<void(const std::string &, JsonBase&)> &func) {
          for(const auto &it : traits_) {
            func(it.first,*it.second);
          }
        }

        /**
         * @brief Cleans up the accquired memory will delete all pointers given
         * to this class in the method insert(...)
         */
        ~JsonImplObject() {
          for(const auto &it : traits_) 
            delete it.second;
        }
      private:
        std::unordered_map<std::string, JsonBase*> traits_; ///< Saves the attribute, Value pair of a json object
    };

    /**
     * @brief The basic implementation of a json string
     */
    class JsonImplString : public JsonInterface  {
      public:
        /**
         * @brief Initialises the string from a given string
         *
         * @param initialise_value The string to initialise the value with
         */
        JsonImplString(std::string initialise_value="") : content_(std::move(initialise_value)) {
        }

        /**
         * @brief Returns JsonType::string as type.
         *
         * @return Return always JsonType::string
         */
        JsonType type() override {
          return JsonType::string;
        }

        /**
         * @brief Returns the size of the container, as this is an primitive
         * type always return 1.
         *
         * @return Returns always 1
         */
        int size() override {
          return 1;
        }

        /**
         * @brief Converts the underlying string into a json string
         *
         * @return Return the string representation of a json string
         */
        std::string dump() override {
          return "\""+content_+"\"";
        }

        /**
         * @brief Returns a copy to the saved string
         *
         * @param err Always leave err untouched
         *
         * @return Returns always the saved string 
         */
        std::string to_string(JsonError &err) override {
          return content_;
        }


      private:
        std::string content_; ///< Saves the content of the json string
    };


    /**
     * @brief The basic implementation of a json null value.
     */
    class JsonImplNull : public JsonInterface  {
      public:
        /**
         * @brief Returns JsonType::string as type.
         *
         * @return Return always JsonType::string
         */
        JsonType type() override {
          return JsonType::null;
        }

        /**
         * @brief Returns the size of the container, as this is an primitive
         * type always return 1.
         *
         * @return Returns always 1
         */
        int size() override {
          return 1;
        }

        /**
         * @brief Converts the underlying string into a json string
         *
         * @return Return the string representation of a json string
         */
        std::string dump() override {
          return "null";
        }
    };


    /**
     * @brief The basic implementation of a json double value
     */
    class JsonImplDouble : public JsonInterface  {
      public:
        /**
         * @brief Initialises the string from a given string
         *
         * @param initialise_value The string to initialise the value with
         */
        JsonImplDouble(double value) : value_(value) {
        }

        /**
         * @brief Returns JsonType::string as type.
         *
         * @return Return always JsonType::string
         */
        JsonType type() override {
          return JsonType::floating_point;
        }

        /**
         * @brief Returns the size of the container, as this is an primitive
         * type always return 1.
         *
         * @return Returns always 1
         */
        int size() override {
          return 1;
        }

        /**
         * @brief Converts the underlying string into a json string
         *
         * @return Return the string representation of a json string
         */
        std::string dump() override {
          std::string k;
          k.reserve(50);
          snprintf(&k[0],k.capacity(),"%f",value_);
          return std::move(k);
        }

        /**
         * @brief Returns the double value and never sets the error field.
         *
         * @param err Never sets the error field to anything
         *
         * @return Returns the saved double
         */
        double to_double(JsonError &err) {
          return value_;
        }



      private:
        double value_; ///< Saves the content of the json string
    };

    /**
     * @brief The basic implementation for a Json array, contains a list of
     * other Json objects
     */
    class JsonImplArray : public JsonInterface {
      public:
        /**
         * @brief Returns the type JsonType::array.
         *
         * @return Always returns the type JsonType::array.
         */
        JsonType type() override {
          return JsonType::array;
        }

        /**
         * @brief Returns the size of the Json array which is always equivalent
         * to the number of items contained.
         *
         * @return The size of the Json array
         */
        int size() override {
          return vec_.size();
        }

        /**
         * @brief Inserts a new value inside the Json array, sets error to
         * JsonError::not_implemented when a key is supplied!
         *
         * @param key The key must always be empty as this is not an Json object
         * @param new_insert The inserted object can be any type
         * @param err Leaves the error untouched if the key is empty, if there
         * is a non empty key sets the error to JsonError::not_implemented
         */
        void insert(const std::string &key, JsonBase *new_insert, JsonError &err) override {
          if (key != "") 
            err = JsonError::not_implemented;
          else 
            vec_.push_back(new_insert);
        }

        /**
         * @brief Returns the json object at the given index, does not perform
         * any boundary checking!!
         *
         * @param index Accesses the index at the given position.
         * @param err Never sets the error to any value
         *
         * @return Returns the Json object at the given index
         */
        JsonBase* get(const int index, JsonError &err) override {
          return vec_[index];
        }

        /**
         * @brief Returns the array converted to a string, also converts all
         * objects in the array to a string 
         *
         * @return Creates a string out of the given Json array 
         */
        std::string dump() override {
          // Return an empty list if there are no members
          if(vec_.size() == 0)
            return "[]";

          // The list starts with [ so set it.
          std::string ret = "[";

          // Dump every json object inside the list
          for (auto it : vec_) {
            ret += std::move(it->dump());

            // Always put a , into the list
            ret += ",";
          }
          // Convert the last , into a closing ]. There always is a , as the
          // list is never empty
          ret[ret.length()-1] = ']';
          // Moves the temporary string back to the callee
          return std::move(ret);
        }

        /**
         * @brief Deletes all items in the array.
         */
        ~JsonImplArray() {
          for(auto it : vec_) 
            delete it;
        }
      private:
        std::vector<JsonBase*> vec_; ///< Saves all the different jsons inside the list
    };

    /**
     * @brief Represents a simple json integer.
     */
  class JsonImplInteger : public JsonInterface  {
      public:
        /**
         * @brief Initialises the JsonInteger with the given value
         *
         * @param value The value to set the json integer to.
         */
        JsonImplInteger(long long value) : value_(value) {
        }

        /**
         * @brief Return the type of integer back to the callee.
         *
         * @return Always return JsonType::integer
         */
        JsonType type() override {
          return JsonType::integer;
        }

        /**
         * @brief As this is an primitive type always return 1.
         *
         * @return Always returns 1
         */
        int size() override {
          return 1;
        }

        /**
         * @brief Converts the integer to a string.
         *
         * @return Returns the integer as an string.
         */
        std::string dump() override {
          return std::to_string(value_);
        }

        /**
         * @brief Returns the saved integer back to the callee, never sets the
         * error field in this function.
         *
         * @param err Never sets the error.
         *
         * @return Returns the saved integer value.
         */
        long long to_int(JsonError &err) override {
          return value_;
        }

      private:
        long long value_; ///< The saved integer value
    };

  /**
   * @brief The basic implementation of a json boolean.
   */
    class JsonImplBoolean : public JsonInterface  {
      public:
        /**
         * @brief Initialises the json boolean to the given value
         *
         * @param value The value to set the underlying boolean to.
         */
        JsonImplBoolean(bool value) : value_(value) {
        }

        /**
         * @brief Returns the type of the Json Boolean.
         *
         * @return Always returns JsonType::boolean.
         */
        JsonType type() override {
          return JsonType::boolean;
        }

        /**
         * @brief As this is an primitive type always return 1.
         *
         * @return Always returns 1.
         */
        int size() override {
          return 1;
        }

        /**
         * @brief Converts the underlying boolean to a string
         *
         * @return Returns "true" or "false" depending on the underlying value
         */
        std::string dump() override {
          if(value_)
            return "true";
          return "false";
        }
        
        /**
         * @brief Returns the saved boolean value and never sets the error
         * field.
         *
         * @param err Never sets the error field in this method.
         *
         * @return Returns the underlying boolean value
         */
        bool to_bool(JsonError &err) override {
          return value_;
        }
      private:
        bool value_;  ///< Returns the saved bool.
    };


    /**
     * @brief Create the default JsonBase, the default json is always an json
     * object
     */
    JsonBase() : interface_(new JsonImplObject), last_error_(JsonError::ok) {
    }

    /**
     * @brief Delete the copy constructor to prevent sharing of ownership which
     * causes segmentation fault.
     *
     * @param base /
     */
    JsonBase(JsonBase &base) = delete;
    
    /**
     * @brief Delete the standard copy assignment operator to prevent sharing of
     * ownership which sometimes causes segmentation fault.
     *
     * @param x /
     *
     * @return /
     */
    JsonBase &operator=(JsonBase &x) = delete;

    /**
     * @brief Implement the move constructor, no ownership problems as the
     * interface is moved away from the other base. The remaining JsonBase is
     * dangerous as the interface points to a nullptr any operation on it will
     * crash the programm.
     *
     * @param base The other Json to move the interface from.
     */
    JsonBase(JsonBase &&base) {
      interface_ = base.interface_;
      last_error_ = base.last_error_;
      base.interface_ = nullptr;
      base.last_error_ = JsonError::ok;
    }

    /**
     * @brief Create the JsonBase with the given interface, one can provide any
     * class that is derive from JsonInterface. The basic types to provide this
     * function with are JsonImplObject, JsonImplBoolean, JsonImplInteger,
     * JsonImplArray
     *
     * @param inter The interface to provide to the json base
     */
    JsonBase(JsonInterface *inter) : interface_(inter), last_error_(JsonError::ok) {
    }

    /**
     * @brief Creates the Json from a given string. This does not PARSE the
     * string but creates a json object of type string. 
     *
     * For example:
     * ```
     * {"hallo": "This is a Json object of type string"}
     *           ______________________________________
     * ```
     *
     * @param x The string to create the json from.
     */
    JsonBase(std::string &&x) : interface_(new JsonImplString(x)), last_error_(JsonError::ok) {
    }

    /**
     * @brief Creates a new Json object of type integer, this just describes an
     * integer inside of a json.
     * For example:
     * ```
     * {"attribute_key": 10240}
     *                   _____  This is an json object of type integer
     * ```
     *
     * @param x Integer to initialise the json with
     */
    JsonBase(long long x) : interface_(new JsonImplInteger(x)), last_error_(JsonError::ok) {
    }

    JsonBase(double x) : interface_(new JsonImplDouble(x)), last_error_(JsonError::ok) {
    }

    /**
     * @brief Creates a new Json object of type boolean.
     * For example:
     * ```
     * {"attribute_key": true}
     *                   ____   This is an json object of type boolean
     * ```
     *
     * @param b The boolean to set the json object to.
     */
    JsonBase(bool b) : interface_(new JsonImplBoolean(b)), last_error_(JsonError::ok) {
    }
    
    /**
     * @brief Destroys the current interface by setting the current interface
     * to nullptr. The function set_interface(...) cleans up dangling pointers.
     */
    ~JsonBase() {
      set_interface(nullptr);
    }

    /**
     * @brief Sets the Json move assignment operator. This is implemented as the
     * move operator avoids ownership conflicts.
     *
     * @param x The Json to move the interface and the error from.
     *
     * @return Returns a reference to itself to enable chain moving.
     */
    JsonBase &operator=(JsonBase &&x) {
      // Sets the new interface and destroys the references to the shared
      // memory.
      set_interface(x.interface_);
      last_error_ = x.last_error_;
      x.interface_ = nullptr;
      return *this;
    }
    
    /**
     * @brief Sets a new interface and cleans up dangling pointers.
     *
     * @param interface The new interface to set the pointer to.
     */
    void set_interface(JsonInterface *interface) {
      if (interface_) {
        delete interface_;
      }
      interface_ = interface;
    }

    /**
     * @brief Returns the underlying type of the current interface.
     *
     * @return The type of the current interface.
     */
    JsonType type() {
      return interface_->type();
    }

    /**
     * @brief Returns the error if there is one.
     *
     * @return The current eror there is.
     */
    JsonError error() {
      return last_error_;
    }

    /**
     * @brief Returns the size of the underlying interface.
     *
     * @return Returns the size of the current interface, for json objects, the
     * number of key:value pairs, for json arrays the number of items in the
     * array.
     */
    int size() {
      return interface_->size();
    }

    /**
     * @brief Convertes the json type to a string format. 
     *
     * @return Returns the json type as string. For example JsonType::object to
     * "JsonType::object".
     */
    std::string type_as_string() {
      // Do a direct mapping of type name to string
      switch(interface_->type()) {
        case JsonType::object:      return "JsonType::object";
        case JsonType::array:       return "JsonType::array";
        case JsonType::integer:     return "JsonType::integer";
        case JsonType::string:      return "JsonType::string";
        case JsonType::boolean:     return "JsonType::boolean";
        default:                    return "JsonType::unknown";
      }
      // This should never happen, but some compilers complain without it.
      return "";
    }

    /**
     * @brief Creates a dump of the given Json
     *
     * @return Returns a string of the underlying json interface.
     */
    std::string dump() {
      return std::move(interface_->dump());
    }

    /**
     * @brief Executes the given function if the type is JsonType::string
     *
     * @param func Call the function with the string inside if the object is of
     * type string
     *
     * @return Returns a reference to itself for function chaining
     */
    JsonBase& map_string(std::function<void(std::string&)> func) {
      std::string tmp = std::move(interface_->to_string(last_error_));
      if (last_error_ == JsonError::ok)
        func(tmp);
      return *this;
    }

    /**
     * @brief Executes the given function if the type is JsonType::int
     *
     * @param func The function to execute when the type is int
     *
     * @return Returns a reference to itself for function chaining
     */
    JsonBase& map_int(std::function<void(int)> func) {
      int tmp = interface_->to_int(last_error_);
      if (last_error_ == JsonError::ok)
        func(tmp);
      return *this;
    }

    /**
     * @brief Executes the given function if the type is JsonType::bool
     *
     * @param func The function to execute when the type fits
     *
     * @return Returns a reference to the instance for function chaining
     */
    JsonBase& map_bool(std::function<void(bool)> func) {
      bool tmp = interface_->to_bool(last_error_);
      if (last_error_ == JsonError::ok)
        func(tmp);
      return *this;
    }

    /**
     * @brief Calls the given function for every object in the array if the
     * instance is an object
     *
     * @param func The function to execute on every member of the array
     *
     * @return Returns a reference to the instance for function chaining
     */
    JsonBase &map_array(std::function<void(JsonBase&)> func) {
      for (int i = 0; i < interface_->size(); i++) {
        JsonBase *val = interface_->get(i,last_error_);
        if(last_error_ == JsonError::ok) {
          func(*val);
        }
        else
          break;
      }
      return *this;
    }

/**
 * @brief Calls the given function on every pair (key,value) in the json object
 *
 * @param func The function to execute on a value pair.
 *
 * @return Returns a reference to itself for easier function chaining
 */
JsonBase &map_object(std::function<void(const std::string&,JsonBase)> func) {
    if (interface_->type() == JsonType::object && last_error_ == JsonError::ok) {
    auto obj = (JsonImplObject*)interface_;
    obj->map_for_each(func);
    }
    else
    last_error_ = JsonError::not_implemented;
    return *this;
    }

    /**
     * @brief Call the given function if no error occured while accessing the
     * array
     *
     * @param func The function to execute.
     *
     * @return Returns a reference to itself for easier function chaining
     */
    JsonBase &map(std::function<void(void)> func) {
    if(!has_error())
    func();
    }


    /**
      * @brief Returns the JsonObject at the given index. This function only
      * works if the underlying type is JsonType::array.
      *
      * @param index The index to return the object from.
      *
      * @return Returns on success the given json object or an instance of this
      * class with an error code set.
    */
    JsonBase &get(int index) {
      auto ret = interface_->get(index,last_error_);
      if (last_error_!=JsonError::ok)
        return *this;
      return *ret;
    }

    /**
      * @brief Returns the json object associated with the given key, if the
      * object does not exist or the key does not exist returns this instance
      * and sets an error code.
      *
      * @param x The key to search for in the json object.
      *
      * @return Either the found JsonBase or this instance depending on the
      * operation outcome.
      */
    JsonBase &get(const std::string &&x) {
      return get(x);
    }

    /**
      * @brief Returns the json object associated with the given key, if the
      * object does not exist or the key does not exist returns this instance
      * and sets an error code.
      *
      * @param x The key to search for in the json object.
      *
      * @return Either the found JsonBase or this instance depending on the
      * operation outcome.
      */
    JsonBase &get(const std::string &x) {
      auto ret = interface_->get(x,last_error_);
      if (last_error_==JsonError::ok)
        return *ret;
      return *this;
    }

/**
 * @brief Tries to insert a new (key,value) pair into an json object
 *
 * @tparam T The type of the inserted value
 * @param x The key to insert into the json object.
 * @param t The value to insert into the Json object.
 *
 * @return Returns an instance to itself. Sets an error code in the class if the
 * object is not of type JsonType::object.
 */
    template<typename T>
    JsonBase &set(const std::string &&x, T t) {
      interface_->insert(x,new JsonBase(t),last_error_);
      return *this;
    }


/**
 * @brief Tries to insert a new (key,const char *) pair. This overload is
 * provided to distinguish between bool and const char* as casting to bool is
 * nearer than casting to const char*
 *
 * @param x The key of the inserted pair
 * @param str The value of the inserted pair, will get inserted as
 * JsonType::string.
 *
 * @return Returns this instance for easier function chaining.
 */
    JsonBase &set(const std::string &&x, const char *str) {
      interface_->insert(x,new JsonBase(std::string(str)),last_error_);
      return *this;
    }


/**
 * @brief Tries to insert a new (key,JsonBase*) pair into a json object. The
 * function sets an error code when the object is not of type json object.
 *
 * @param x The key to insert into the JsonObject.
 * @param js The Interface to insert into the JsonObject.
 *
 * @return Returns an instance of itself for easier function chaining.
 */
    JsonBase &set(const std::string &&x, JsonBase *js) {
      interface_->insert(x,js,last_error_);
      return *this;
    }

/**
 * @brief Tries to insert a new value into the json array. Sets an error code if
 * the JsonBase is not of type JsonType::array
 *
 * @tparam T The type to insert into the json object. Every type is insertable
 * which can be constructed by JsonBase(T);
 * @param t The value to call the constructor with.
 *
 * @return An instance of itself for easier function chaining
 */
    template<typename T>
    JsonBase &push_back(T t) {
      interface_->insert("",new JsonBase(t),last_error_);
      return *this;
    }

/**
 * @brief Tries to insert a string into the json array. If the JsonBase is not
 * of type JsonType::array then an error code is set.
 *
 * @param str The string to insert into the array.
 *
 * @return Returns an instance to itself for easier function chaining
 */
    JsonBase &push_back(const char *str) {
      interface_->insert("",new JsonBase(std::string(str)),last_error_);
      return *this;
    }

/**
 * @brief Inserts an interface into the json array. An error code is set if the
 * underlying type is not JsonType::array.
 *
 * @param js The interface to include into the json array.
 *
 * @return Returns an instance to itself for easier function chaining.
 */
    JsonBase &push_back(JsonBase *js) {
      interface_->insert("",js,last_error_);
      return *this;
    }

/**
 * @brief Sets the internal error state to the given error code. If there is
 * already an error code set this function will just silently overwrite it.
 *
 * @param err The error code to set the last_error to.
 */
    void set_error(JsonError err) {
      last_error_ = err;
    }

/**
 * @brief Checks if there was an illegal call or cast using this instance.
 *
 * @return True if there happened an illegal action.
 */
    bool has_error() {
      return last_error_ != JsonError::ok;
    }

/**
 * @brief Calls the callback function if there was an illegal operation or an
 * error in this instance.
 *
 * @param func The function to call when there is an error inside.
 */
    void error(std::function<void(JsonError)> func) {
      if (last_error_ != JsonError::ok) {
        func(last_error_);
        last_error_ = JsonError::ok;
      }
    }

/**
 * @brief The JsonParser used to parse large json strings.
 */
    struct JsonParser {


      /**
       * @brief Possible json parse errors while parsing.
       */
      enum class JsonParserError {
        ok,
        expected_comma_before_next_attribute,
        expected_comma_before_next_array_item,
        expected_attribute_but_got_comma,
        expected_string_attribute_key,
        expected_closing_quote_but_got_eos,
        expected_beginning_of_string_int_object_or_array_null_float,
        expected_colon_but_got_different_character_instead,
        expected_int_or_double,
      };
      
      int abs_pos_;   ///< The current absolute position in the underlying_json_ string view.
      JsonParserError error_;   ///< The current state, everything other than JsonParserError::ok means abort parsing.
      std::string_view underlying_json_;  ///< The json to parse.

      
      /**
       * @brief Parses the given json starting at startpos
       *
       * @param view The string to parse, using a string view to save
       * ressources.
       * @param startpos The starting position.
       */
      JsonParser(std::string_view view,int startpos=0) {
        underlying_json_ = view;
        abs_pos_ = startpos;
        // No error at the beginning
        error_ = JsonParserError::ok;
      }

      /**
       * @brief Checks if there has been a parse error.
       *
       * @return True if there has been a parse error false otherwise.
       */
      bool parse_error() {
        return error_ != JsonParserError::ok;
      }

      /**
       * @brief Sets the current parse error to the given value.
       *
       * @param err Sets the parse error to the given parse error.
       */
      void set_error(JsonParserError err) {
        error_ = err;
      }

      /**
       * @brief Tries to find the error by taking a substr arround the last
       * parsed position to show the user the error.
       *
       * @param dist The distance in both directions to cut out of the json.
       *
       * @return The substr arround the parse error to show the user the error.
       */
      std::string get_error_surroundings(int dist) {
        int start = (abs_pos_-dist<0)?0:abs_pos_-dist;
        int end = (abs_pos_+dist>=underlying_json_.length())?underlying_json_.length():abs_pos_+dist;
        return std::string(underlying_json_.substr(start,end-start));
      }

      /**
       * @brief Converts the given parse error into a string
       *
       * @return The string version of the error code.
       */
      std::string get_error_string() {
        switch(error_) {
          case JsonParserError::ok:
            return "No error.";
          case JsonParserError::expected_comma_before_next_attribute:
            return "Expected ',' before the next attribute in Json::object.";
          case JsonParserError::expected_comma_before_next_array_item:
            return "Expected ',' before next item in the Json::array";
          case JsonParserError::expected_attribute_but_got_comma:
            return "Expected next attribute but got ',' instead.";
          case JsonParserError::expected_string_attribute_key:
            return "Expected string attribute key but could not find string or the string was empty.";
          case JsonParserError::expected_closing_quote_but_got_eos:
            return "Expected closing quotes but got end of string instead.";
          case JsonParserError::expected_beginning_of_string_int_object_or_array_null_float:
            return "Expected beginning of new json type either, string, int, bool, array, null, float or object, but got something else.";
          case JsonParserError::expected_colon_but_got_different_character_instead:
            return "Expected colon after Json::object attribute key but got different character instead.";
          case JsonParserError::expected_int_or_double:
            return "Expected double or integer but got different character.";
        }
        return "Unknown error.";
      }

      /**
       * @brief Parses an json object therefore exepects a specific type
       *
       * @return Returns the created json object
       */
      JsonBase *parse_object() {
        //Skip the first character which we know is {
        ++abs_pos_;

        std::string key;
        // We always return a valid object!
        JsonBase *object = new JsonBase();

        //No comma expected yet first we need a key.
        bool expect_comma = false;

        for (;abs_pos_ < underlying_json_.length(); abs_pos_++) {
          //Skip all trailing characters as we are not in a string this can be
          //skipped safely
          if (underlying_json_[abs_pos_] == ' ' || underlying_json_[abs_pos_] == '\t' || underlying_json_[abs_pos_] == '\n')
            continue;
          // Check the finish tag if we find this the object is finished.
          else if (underlying_json_[abs_pos_] == '}')
            break;

          // Check the comma, if there is one the next (key,value) pair can
          // start.
          else if (underlying_json_[abs_pos_] == ',') {
            // Ok we did not have (key,value) pair before , set error and exit
            if (!expect_comma) {
              set_error(JsonParserError::expected_attribute_but_got_comma);
              break;
            }
            //Ok we just got a comma, we dont expect another one yet.
            expect_comma = false;
          }
          // No key yet? Expect JsonString then.
          else if (key.length() == 0) {

            // If we expect a comma now there is a missing comma between
            // (key,value) and (key1,value1)
            if (expect_comma) {
              set_error(JsonParserError::expected_comma_before_next_attribute);
              break;
            }

            //Parse the next json should be a string.
            JsonBase *json_key = parse();

            // Write the string into key by moving it.
            json_key->map_string([&key](std::string x){key = std::move(x);});
            // Delete the json_key we dont need it no more.
            delete json_key;
            // Empty keys are invalid in the Json standard.
            if (key == "")
              set_error(JsonParserError::expected_string_attribute_key);
          }
          else {
            // Ok we have a key already expect a double colon now. The first if
            // should filter all filling characters therefore the next one needs
            // to be a double colon.
            if (underlying_json_[abs_pos_++]!=':') {
              set_error(JsonParserError::expected_colon_but_got_different_character_instead);
              break;
            }

            // Parse the value either string, null ....
            JsonBase *json_item = parse();
            // Set the object in the objects.
            object->set(std::move(key),json_item);

            //Reset keys and expect comma next.
            key = "";
            expect_comma = true;
          }

          // Check if there was an error if so no need to go on.
          if(error_ != JsonParserError::ok)
            break;
        }
        return object;
      }

      /**
       * @brief Skips all filling characters in the underlying json must not be
       * called from inside json string!
       */
      void skip_whitespace_tab_newline() {
        while(underlying_json_[abs_pos_]==' ' || underlying_json_[abs_pos_] == '\t' || underlying_json_[abs_pos_] == '\n')
          ++abs_pos_;
      }

      /**
       * @brief Parses an json array and returns them. Always returns a value !=
       * nullptr
       *
       * @return Returns the parsed json array.
       */
      JsonBase *parse_array() {
        //Skip the first character which we know is [
        ++abs_pos_;

        //Always return a value != nullptr.
        JsonBase *array = new JsonBase(new JsonImplArray());

        // No comma is expected yet.
        bool expect_comma = false;

        for (;abs_pos_< underlying_json_.length(); abs_pos_++) {
          // Skip all filling characters.
          skip_whitespace_tab_newline();
          // We are finished on the closing character.
          if (underlying_json_[abs_pos_] == ']')
            break;
          // If there is an comma set the expected comma or error depending on
          // the expected value.
          else if (underlying_json_[abs_pos_] == ',') {
            if (!expect_comma) {
              set_error(JsonParserError::expected_comma_before_next_array_item);
              break;
            }
            expect_comma = false;
          }
          else {
            // Parse the item and add it to the array.
            JsonBase *json_item = parse();
            array->push_back(json_item);
            expect_comma = true;
          }

          if (error_ != JsonParserError::ok)
            break;
        }
        return array;
      }

      /**
       * @brief Parses the next json string
       *
       * @return The parsed json string.
       */
      JsonBase *parse_string() {
        //Skip the first character which we know is "
        ++abs_pos_;
        std::string the_content;

        for (;abs_pos_< underlying_json_.length(); abs_pos_++) {
          // Add everything into the JsonBase as long as there are characters
          // and no closing characters skip all \" and \\" characters.
          if (underlying_json_[abs_pos_] == '"' && underlying_json_[abs_pos_-1] != '\\') {
            return new JsonBase(std::move(the_content));
          }
          the_content+=underlying_json_[abs_pos_];
        }
        //Something went wrong no closing quote until json end.
        set_error(JsonParserError::expected_closing_quote_but_got_eos);
        return new JsonBase(std::move(the_content));
      }


      /**
       * @brief Parses either an json integer or an json double
       *
       * @return 
       */
      JsonBase *parse_integer_or_double() {
        //Skip the first character which we know is "
        int beginning = abs_pos_;
        bool is_float = false;

        for (;abs_pos_< underlying_json_.length(); abs_pos_++) {
          //Check the sign flag, is not needed as strtod will check it as well
          if (underlying_json_[abs_pos_] == '-') {
          }
          // Is floating point?
          else if(underlying_json_[abs_pos_] == '.')
            is_float = true;
          // Check the end of the integer or double
          else if (underlying_json_[abs_pos_] > '9' || underlying_json_[abs_pos_] < '0') {
            break;
          }
        }
        // The end should be the last valid character
        --abs_pos_;
        
        const char *start = &underlying_json_[beginning];
        char *end;
        JsonInterface *impl;

        // Cast the given string to int or double
        if (is_float) {
          double val = strtod(start,&end);
          impl = new JsonImplDouble(val);
        }
        else {
          long val = strtol(start,&end,10);
          impl = new JsonImplInteger(val);
        }

        //Cast error
        if (start == end)
          set_error(JsonParserError::expected_int_or_double);
        return new JsonBase(impl);
      }

      /**
       * @brief Parse boolean value.
       *
       * @return Returns the created JsonBase bool object.
       */
      JsonBase *parse_boolean() {
        bool value;

        if (underlying_json_.substr(abs_pos_,4) == "true") {
          abs_pos_ = abs_pos_+3;
          value = true;
        }
        else {
          abs_pos_ = abs_pos_+4;
          value = false;
        }
        return new JsonBase(value);
      }

      /**
       * @brief Check the json null value.
       *
       * @return Returns the created JsonBase null object.
       */
      JsonBase *parse_null() {
        if(std::string_view(&underlying_json_[abs_pos_],4) != "null")
          set_error(JsonParserError::expected_beginning_of_string_int_object_or_array_null_float);
        abs_pos_+=3;
        return new JsonBase(new JsonImplNull());
      }


      /**
       * @brief Parses a json.
       *
       * @return The parsed in DOM format.
       */
      JsonBase *parse() {
        JsonBase *inter = nullptr;

        for (; abs_pos_ < underlying_json_.length(); abs_pos_++) {
          if (underlying_json_[abs_pos_] == ' ' || underlying_json_[abs_pos_] == '\t' || underlying_json_[abs_pos_] == '\n')
            continue;
          else
            break;
        }

        // Check the type of the comming object.
        switch (underlying_json_[abs_pos_]) {
          case '{':
            inter = parse_object();
            break;
          case '[':
            inter = parse_array();
            break;
          case '"':
            inter = parse_string();
            break;
          case 't':
          case 'f':
            inter = parse_boolean();
            break;
          case 'n':
            inter = parse_null();
            break;
          default:
            if ((underlying_json_[abs_pos_] <= '9' && underlying_json_[abs_pos_] >= '0') || underlying_json_[abs_pos_] == '-')
              inter = parse_integer_or_double();
            else {
              set_error(JsonParserError::expected_beginning_of_string_int_object_or_array_null_float);
              return new JsonBase();
            }
        }
        return inter;
      }
    };


      /**
       * @brief Parses any json string into a JSON DOM object
       *
       * @param view The json string to parse
       * @param on_error The callback to call when there is an error.
       *
       * @return Returns the parsed json in dom format.
       */
    static JsonBase parse(std::string_view view,std::function<void(JsonParser&)> on_error) {
      JsonParser parser(view);
      auto result = parser.parse();
      
      JsonBase base(std::move(*result));
      delete result;
      
      if (parser.parse_error()) {
        on_error(parser);
        base.set_error(JsonError::parse_error);
      }

      return std::move(base);
    }

  private:
    JsonInterface *interface_;  ///< The underlying interface, this class is polymorphic to enable custom types and advancements.
    JsonError last_error_;      ///< This variable stores the last error that happended to enable error callbacks and function style assignments
};

///< Setting the type to the json can be used like this Json b;
using Json = JsonBase<JsonLogLevel::none,JsonStdoutColoredFunctor>;


#endif
