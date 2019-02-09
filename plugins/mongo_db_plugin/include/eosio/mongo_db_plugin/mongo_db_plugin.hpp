/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/http_plugin/http_plugin.hpp>
#include <appbase/application.hpp>
#include <memory>

namespace eosio {

using mongo_db_plugin_impl_ptr = std::shared_ptr<class mongo_db_plugin_impl>;

    namespace mongo_db_apis {

        class read_write {
            mongo_db_plugin_impl_ptr mongo_db;

        public:
            read_write(mongo_db_plugin_impl_ptr&& mongo_db)
                    : mongo_db(mongo_db) {}


            struct filter_on_accounts_params {
                vector<chain::account_name> account_names;
            };

            struct filter_on_accounts_result {
                int32_t     new_accounts;
            };

            filter_on_accounts_result filter_on_accounts( const filter_on_accounts_params& )const;
        };


    } // namespace mongo_db_apis

/**
 * Provides persistence to MongoDB for:
 * accounts
 * actions
 * block_states
 * blocks
 * transaction_traces
 * transactions
 * pub_keys
 * account_controls
 *
 *   See data dictionary (DB Schema Definition - EOS API) for description of MongoDB schema.
 *
 *   If cmake -DBUILD_MONGO_DB_PLUGIN=true  not specified then this plugin not compiled/included.
 */
class mongo_db_plugin : public plugin<mongo_db_plugin> {
public:
   APPBASE_PLUGIN_REQUIRES((chain_plugin))

   mongo_db_plugin();
   virtual ~mongo_db_plugin();

   virtual void set_program_options(options_description& cli, options_description& cfg) override;

   void plugin_initialize(const variables_map& options);
   void plugin_startup();
   void plugin_shutdown();

   mongo_db_apis::read_write  get_read_write_api()const { return mongo_db_apis::read_write(mongo_db_plugin_impl_ptr(my)); }

private:
   mongo_db_plugin_impl_ptr my;
};

using namespace appbase;

class mongo_db_api_plugin : public plugin<mongo_db_api_plugin> {
    public:
        APPBASE_PLUGIN_REQUIRES((mongo_db_plugin)(chain_plugin))

    mongo_db_api_plugin();
        virtual ~mongo_db_api_plugin();

        virtual void set_program_options(options_description&, options_description&) override;

        void plugin_initialize(const variables_map&);
        void plugin_startup();
        void plugin_shutdown();

    private:
};
}

FC_REFLECT( eosio::mongo_db_apis::read_write::filter_on_accounts_params, (account_names) )
FC_REFLECT( eosio::mongo_db_apis::read_write::filter_on_accounts_result, (new_accounts) )
