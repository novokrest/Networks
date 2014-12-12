package spbau.novokreshchenov.network.hw1;

import spbau.novokreshchenov.network.hw1.Exceptions.BankNotFoundException;
import spbau.novokreshchenov.network.hw1.Exceptions.ExchangeRateAlreadyExistsException;
import spbau.novokreshchenov.network.hw1.Exceptions.ExchangeRateNotFoundException;

import java.util.HashMap;
import java.util.Map;


public class ExchangeRateSearcher {
    private final Map<String, Currency> currencies = new HashMap<String, Currency>();
    private final Map<String, Bank> banks = new HashMap<String, Bank>();

    public ExchangeRateSearcher() {
        Currency dollar = new Currency("dollar", "dollar-icon.png");
        Currency euro = new Currency("euro", "euro-icon.png");
        addCurrency(dollar);
        addCurrency(euro);

        Bank sberbank =  new Bank("sberbank", "sberbank-icon.png");
        sberbank.putExchangeRate(dollar.getName(), 55.0f, 60.0f);
        sberbank.putExchangeRate(euro.getName(), 65.0f, 70.0f);

        Bank raiffeisenbank = new Bank("raiffeisenbank", "raiffeisen-icon.jpg");
        raiffeisenbank.putExchangeRate(dollar.getName(), 59.0f, 65.0f);
        raiffeisenbank.putExchangeRate(euro.getName(), 70.0f, 71.0f);

        Bank citibank = new Bank("citibank", "citibank-icon.jpg");
        citibank.putExchangeRate(dollar.getName(), 51.0f, 54.0f);
        citibank.putExchangeRate(euro.getName(), 61.0f, 62.0f);

        addBank(sberbank);
        addBank(raiffeisenbank);
        addBank(citibank);
    }

    public void addBank(Bank bank) {
        banks.put(bank.getName(), bank);
    }

    public Bank getBankByName(String bankName) {
        return banks.get(bankName);
    }

    public void addCurrency(Currency currency) {
        currencies.put(currency.getName(), currency);
    }

    public Currency getCurrencyByName(String currency) {
        return currencies.get(currency);
    }

    public ExchangeRate getExchangeRate(String bankName, String currency) throws ExchangeRateNotFoundException, BankNotFoundException {
        if (banks.containsKey(bankName)) {
            Bank bank = banks.get(bankName);
            return bank.getExchangeRate(currency);
        }

        throw new BankNotFoundException(bankName);
    }

    public void addExchangeRate(String bankName, String currency, float buyValue, float sellValue) throws ExchangeRateNotFoundException, ExchangeRateAlreadyExistsException, BankNotFoundException {
        if (banks.containsKey(bankName)) {
            Bank bank = banks.get(bankName);
            bank.addExchangeRate(currency, buyValue, sellValue);
        }

        throw new BankNotFoundException(bankName);
    }

    public void updateExchangeRate(String bankName, String currency, float buyValue, float sellValue) throws ExchangeRateNotFoundException, BankNotFoundException {
        if (banks.containsKey(bankName)) {
            Bank bank = banks.get(bankName);
            bank.updateExchangeRate(currency, buyValue, sellValue);
        }

        throw new BankNotFoundException(bankName);
    }
}
