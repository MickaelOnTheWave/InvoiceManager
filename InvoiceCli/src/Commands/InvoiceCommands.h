/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INVOICECOMMANDS_H
#define INVOICECOMMANDS_H

#include "AbstractCommand.h"

class InvoiceCommand : public AbstractCommand
{
public:
   InvoiceCommand(const std::string& cliParamName);

   void Run(InvoiceDbController& controller,
            const CommandLineManager& cli) override;
private:
   virtual void RunOnInvoice(const int id, const InvoiceDbController& controller) = 0;
};

class ShowCommand : public InvoiceCommand
{
public:
   ShowCommand(const std::string& cliParamName);

private:
   void RunOnInvoice(const int id, const InvoiceDbController& controller) override;
};

class GetHtmlCommand : public InvoiceCommand
{
public:
   GetHtmlCommand(const std::string& cliParamName);

private:
   void RunOnInvoice(const int id, const InvoiceDbController& controller) override;
};

class GetCssCommand : public InvoiceCommand
{
public:
   GetCssCommand(const std::string& cliParamName);

private:
   void RunOnInvoice(const int id, const InvoiceDbController& controller) override;
};

class GetStyledHtmlCommand : public InvoiceCommand
{
public:
   GetStyledHtmlCommand(const std::string& cliParamName);

private:
   void RunOnInvoice(const int id, const InvoiceDbController& controller) override;
};


#endif
