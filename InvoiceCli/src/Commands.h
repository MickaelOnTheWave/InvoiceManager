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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "commandlinemanager.h"
#include "InvoiceDbController.h"

class ShowCommand
{
public:
   static void Run(const InvoiceDbController& controller,
                       const CommandLineManager& cli);
};

class GetHtmlCommand
{
public:
   static void Run(const InvoiceDbController& controller,
                       const CommandLineManager& cli);
};

class GetCssCommand
{
public:
   static void Run(const InvoiceDbController& controller,
                       const CommandLineManager& cli);
};

class GetStyledHtmlCommand
{
public:
   static void Run(const InvoiceDbController& controller,
                       const CommandLineManager& cli);
};


#endif
