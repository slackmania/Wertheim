/**
 * WERTHEIM Cipher — Public API barrel
 *
 * Re-exports all public functions from the Wertheim cipher module.
 * Usage:
 *   import { hintabe, hintaki, encode64, decode64, CODES64, ABC0 } from "./index.js";
 */

export {
    hintabe,
    hintaki,
    encode64,
    decode64,
    besecure,
    kisecure,
    bekod,
    kikod,
    pw_to_kod,
    genhinta,
    CODES64,
    ABC0,
    MAX_PWD_LEN,
} from "./wertheim.js";
