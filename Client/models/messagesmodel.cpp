#include "messagesmodel.h"
#include <QPixmap>
#include <QUrl>
#include <QDir>

using namespace Models;

MessagesModel::MessagesModel(QObject *parent)
	: QAbstractListModel(parent)
	, m_messages
	{
		{ QDateTime(QDate::currentDate(), QTime::currentTime()), "111", "Ivan Ivlev", "Message", MessageAuthor::Me, QUrl::fromLocalFile("Vanya.jpg") },
		{ QDateTime(QDate::currentDate().addDays(1), QTime::currentTime()), "111", "Pavel Zharov", "Answer\n\n\n\n\n111", MessageAuthor::Other, QUrl::fromLocalFile("Pasha.jpg") }
	}
{

}

MessagesModel::~MessagesModel()
{

}

bool MessagesModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
	return !parent.isValid() && column == 0 && 
		row >= 0 && row < m_messages.size();
}

QModelIndex MessagesModel::index(int row, int column, const QModelIndex& parent) const
{
	assert(!parent.isValid());

	return createIndex(row, column, static_cast<quintptr>(0));
}

QModelIndex MessagesModel::parent(const QModelIndex& child) const
{
	assert(child.isValid());

	return {};
}

int MessagesModel::rowCount(const QModelIndex& parent) const
{
	assert(!parent.isValid());

	return static_cast<int>(m_messages.size());
}

int MessagesModel::columnCount(const QModelIndex& parent) const
{
	assert(!parent.isValid());

	return 1;
}

bool MessagesModel::hasChildren(const QModelIndex& parent) const
{
	return !parent.isValid() && rowCount(parent) > 0;
}

QVariant MessagesModel::data(const QModelIndex& index, int role) const
{
	assert(hasIndex(index.row(), index.column(), index.parent()));

	switch (role)
	{
	case Qt::DisplayRole:
		return { m_messages[index.row()].text };

	case MessagesDataRole::MessageAuthorRole:
		return { m_messages[index.row()].name };

	case MessagesDataRole::MessageTimeRole:
		{
			auto dateTime = m_messages[index.row()].dateTime;
			if (dateTime.date() == QDate::currentDate())
			{
				return { dateTime.time().toString("hh:mm") };
			}
			return { dateTime.toString("dd.MM.yy hh:mm") };
		}

	case MessagesDataRole::MessageAvatarRole:
		return { m_messages[index.row()].avatar };

	case MessagesDataRole::MessageIsFromMeRole:
		return { m_messages[index.row()].author == MessageAuthor::Me };

	default: 
		return {};
	}
}

bool MessagesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return false;
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
	return
	{
		{ Qt::DisplayRole, "messageText" },
		{ MessagesDataRole::MessageAuthorRole, "messageName" },
		{ MessagesDataRole::MessageTimeRole, "messageTime" },
		{ MessagesDataRole::MessageAvatarRole, "messageAvatar" },
		{ MessagesDataRole::MessageIsFromMeRole, "messageIsFromMe" }
	};
}
