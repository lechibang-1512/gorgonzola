#include "main/vector_extension.h"

#include "catalog/hnsw_index_catalog_entry.h"
#include "function/hnsw_index_functions.h"
#include "main/client_context.h"
#include "main/database.h"

namespace gorgonzola {
namespace vector_extension {

static void initHNSWEntries(main::ClientContext* context) {
    auto catalog = catalog::Catalog::Get(*context);
    for (auto& indexEntry : catalog->getIndexEntries(transaction::Transaction::Get(*context))) {
        if (indexEntry->getIndexType() == HNSWIndexCatalogEntry::TYPE_NAME &&
            !indexEntry->isLoaded()) {
            // Deserialize lightweight catalog metadata eagerly.
            indexEntry->setAuxInfo(HNSWIndexAuxInfo::deserialize(indexEntry->getAuxBufferReader()));
            // NOTE (#6047): The heavy storage-level index loading is intentionally deferred.
            // Previously, unloadedIndex.load() was called here for every HNSW index,
            // which blocked database startup for seconds with large indexes.
            // The index will now be loaded lazily when first queried via QUERY_VECTOR_INDEX.
        }
    }
}


void VectorExtension::load(main::ClientContext* context) {
    auto& db = *context->getDatabase();
    extension::ExtensionUtils::addTableFunc<QueryVectorIndexFunction>(db);
    extension::ExtensionUtils::addInternalStandaloneTableFunc<InternalCreateHNSWIndexFunction>(db);
    extension::ExtensionUtils::addInternalStandaloneTableFunc<InternalFinalizeHNSWIndexFunction>(
        db);
    extension::ExtensionUtils::addStandaloneTableFunc<CreateVectorIndexFunction>(db);
    extension::ExtensionUtils::addInternalStandaloneTableFunc<InternalDropHNSWIndexFunction>(db);
    extension::ExtensionUtils::addStandaloneTableFunc<DropVectorIndexFunction>(db);
    extension::ExtensionUtils::registerIndexType(db, OnDiskHNSWIndex::getIndexType());
    initHNSWEntries(context);
}

} // namespace vector_extension
} // namespace gorgonzola

#if defined(BUILD_DYNAMIC_LOAD)
extern "C" {
// Because we link against the static library on windows, we implicitly inherit GORGONZOLA_STATIC_DEFINE,
// which cancels out any exporting, so we can't use GORGONZOLA_API.
#if defined(_WIN32)
#define INIT_EXPORT __declspec(dllexport)
#else
#define INIT_EXPORT __attribute__((visibility("default")))
#endif
INIT_EXPORT void init(gorgonzola::main::ClientContext* context) {
    gorgonzola::vector_extension::VectorExtension::load(context);
}

INIT_EXPORT const char* name() {
    return gorgonzola::vector_extension::VectorExtension::EXTENSION_NAME;
}
}
#endif
